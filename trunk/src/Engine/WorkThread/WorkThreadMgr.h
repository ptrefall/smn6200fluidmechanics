#pragma once

namespace Engine
{
	class WorkProducer;
	class WorkProduction;
	class WorkDoneData;
	class WorkData;
	class WorkProductionDoneSync;

	class CoreMgr;
	class WorkThreadMgr
	{
	public:
		WorkThreadMgr(CoreMgr *coreMgr);
		~WorkThreadMgr();

		bool isWorkGroupCompletedFor(WorkProducer *producer);
		WorkDoneData *getWorkGroupDoneData(WorkProducer *producer);

		void addWorkGroup(WorkProducer *producer, std::vector<WorkData*> work_group, WorkDoneData *doneData);

		void finishedWork(WorkProducer *producer, unsigned int index);

		void update(float dt);
	private:
		void worker_main(unsigned int core);
		void process_work(unsigned int core);
		void queue(WorkData *data);

		void wait_for_workers();
		void wait_for_queue_space();
		void update_local_worker_index();
		
		std::map<WorkProducer*, WorkProduction*> produce;
		std::vector<WorkData*> work_queue;

		unsigned int active_cores;
		std::vector<CL_Thread> worker_threads;
		std::vector<CL_Event> event_more_work;
		CL_Event event_worker_done;
		CL_Event event_stop;

		enum { queue_max = 200000, work_threshold = 1 };
		int local_queue_index;
		int local_worker_index;
		int local_work_queued;

		CL_InterlockedVariable queue_index;
		std::vector<CL_InterlockedVariable> worker_indices;
		std::vector<CL_InterlockedVariable> worker_active;
		
		std::vector<WorkProductionDoneSync*> jobDoneSync;
		std::vector<CL_Event> event_jobDoneSync;
		bool waiting_for_job_done;

		CoreMgr *coreMgr;
	};
}
