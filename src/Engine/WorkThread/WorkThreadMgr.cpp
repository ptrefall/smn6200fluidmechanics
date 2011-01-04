#include "precomp.h"
#include "WorkThreadMgr.h"
#include "WorkProduction.h"
#include "WorkProducer.h"
#include "WorkData.h"
#include "WorkProductionDoneSync.h"

using namespace Engine;

#ifdef _MSC_VER
	#include <intrin.h>
	#ifndef compiler_barrier
		#define compiler_barrier() _ReadWriteBarrier()
	#endif
#else
	#ifndef compiler_barrier
		#define compiler_barrier()  __asm__ __volatile__("" : : : "memory")
	#endif
#endif

WorkThreadMgr::WorkThreadMgr(CoreMgr *coreMgr)
: coreMgr(coreMgr), active_cores(0), local_queue_index(0), local_worker_index(0), local_work_queued(0)
{
	active_cores = (unsigned int)CL_System::get_num_cores();

	worker_indices.resize(active_cores);
	worker_active.resize(active_cores);

	work_queue.resize(queue_max, 0);

	for(unsigned int core = 0; core < active_cores; core++)
	{
		event_more_work.push_back(CL_Event());
	}

	CL_Console::write_line(cl_format("Spawning %1 worker threads", active_cores));
	for(unsigned int core = 0; core < active_cores; core++)
	{
		CL_Thread worker_thread;
		worker_thread.start<WorkThreadMgr, unsigned int>(this, &WorkThreadMgr::worker_main, core);
		worker_threads.push_back(worker_thread);
	}
}

WorkThreadMgr::~WorkThreadMgr()
{
	wait_for_workers();
	event_stop.set();

	for(unsigned int core = 0; core < worker_threads.size(); core++)
	{
		worker_threads[core].join();
	}

	for(unsigned int i = 0; i < queue_max; i++)
	{
		if(work_queue[i] != 0)
		{
			delete work_queue[i];
			work_queue[i] = 0;
		}
	}
}

void WorkThreadMgr::addWorkGroup(WorkProducer *producer, std::vector<WorkData*> work_group, WorkDoneData *doneData)
{
	waiting_for_job_done = true;
	WorkProduction *production = new WorkProduction(producer, work_group, doneData);
	produce[producer] = production;
	producer->insertProduction(production);

	//Queue work
	CL_Console::write_line(cl_format("Filling work queue with %1 jobs", work_group.size()));
	for(unsigned int i = 0; i < work_group.size(); i++)
	{
		queue(work_group[i]);
	}
	
	WorkProductionDoneSync *syncJobDone = new WorkProductionDoneSync(producer, active_cores);
	jobDoneSync.push_back(syncJobDone);
	event_jobDoneSync.push_back(syncJobDone->done_event);
	queue(syncJobDone);
	
	CL_Console::write_line("Finished filling work queue with jobs");

	int wakeup_reason = CL_Event::wait(event_jobDoneSync, 100);
	if(wakeup_reason >= 0)
	{
		event_jobDoneSync[wakeup_reason].reset();
		CL_Console::write_line("Job finished!");
		jobDoneSync[wakeup_reason]->getProducer()->finished(doneData);
		waiting_for_job_done = false;
	}
}

void WorkThreadMgr::update(float dt)
{
	if(waiting_for_job_done)
	{
		int wakeup_reason = CL_Event::wait(event_jobDoneSync, 100);
		if(wakeup_reason >= 0)
		{
			event_jobDoneSync[wakeup_reason].reset();
			CL_Console::write_line("Job finished!");
			WorkProducer *producer = jobDoneSync[wakeup_reason]->getProducer();
			WorkProduction *production = producer->getProduction();
			producer->finished(production->getDoneData());
			waiting_for_job_done = false;
		}
	}
}

void WorkThreadMgr::queue(WorkData *data)
{
	wait_for_queue_space();
	work_queue[local_queue_index] = data;
	
	local_queue_index++;
	if(local_queue_index == queue_max)
		local_queue_index = 0;
	local_work_queued++;

	if(local_work_queued == work_threshold)
	{
		compiler_barrier();
		queue_index.set(local_queue_index);
		local_work_queued = 0;

		for(unsigned int core = 0; core < active_cores; core++)
		{
			if(worker_active[core].get() == 0)
				event_more_work[core].set();
		}
	}
}

void WorkThreadMgr::wait_for_workers()
{
	if (local_work_queued > 0)
	{
		compiler_barrier();
		queue_index.set(local_queue_index);
		local_work_queued = 0;

		for (unsigned int core = 0; core < active_cores; core++)
			event_more_work[core].set();
	}

	if (local_worker_index != local_queue_index)
	{
		update_local_worker_index();
		while (local_worker_index != local_queue_index)
		{
			event_worker_done.wait();
			event_worker_done.reset();
			update_local_worker_index();
		}
	}
}

void WorkThreadMgr::wait_for_queue_space()
{
	int next_index = local_queue_index+1;
	if (next_index == queue_max)
		next_index = 0;
	if (next_index == local_worker_index)
	{
		update_local_worker_index();
		while (next_index == local_worker_index)
		{
			event_worker_done.wait();
			event_worker_done.reset();
			update_local_worker_index();
		}
	}
}

void WorkThreadMgr::update_local_worker_index()
{
	local_worker_index = local_queue_index;
	for (unsigned int core = 0; core < active_cores; core++)
	{
		int worker_index = worker_indices[core].get();
		if (worker_index > local_queue_index)
			worker_index -= queue_max;
		if (worker_index < local_worker_index)
			local_worker_index = worker_index;
	}
	if (local_worker_index < 0)
		local_worker_index += queue_max;
}

void WorkThreadMgr::worker_main(unsigned int core)
{
	CL_Thread::set_thread_name(cl_format("Worker_%1", core).c_str());

	while(true)
	{
		int wakeup_reason = CL_Event::wait(event_more_work[core], event_stop);
		if(wakeup_reason != 0)
			break;

		event_more_work[core].reset();
		process_work(core);
	}
}

void WorkThreadMgr::process_work(unsigned int core)
{
	while(true)
	{
		int worker_index = worker_indices[core].get();
		int worker_queue_index = queue_index.get();
		int worker_job_retired = 0;
		if (worker_index == worker_queue_index)
			break;

		worker_active[core].set(1);
		{
			while (worker_index != worker_queue_index)
			{
				// HANDLE WORK DATA
				WorkData *data = work_queue[worker_index];
				bool handled = true;
				{
					compiler_barrier();
					handled = data->isHandled();
				}
				if(handled == false)
				{
					data->handle();
				}

				worker_index++;
				if (worker_index == queue_max)
					worker_index = 0;
				worker_job_retired++;

				if (worker_job_retired == work_threshold)
				{
					compiler_barrier();
					worker_indices[core].set(worker_index);
					worker_job_retired = 0;
					event_worker_done.set();
				}
			}

			if (worker_job_retired > 0)
			{
				compiler_barrier();
				worker_indices[core].set(worker_index);
				worker_job_retired = 0;
				event_worker_done.set();
			}
		}
		worker_active[core].set(0);
	}
}
