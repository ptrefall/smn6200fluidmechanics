#pragma once

#include "WorkData.h"

namespace Engine
{
	class WorkProducer;
	class WorkProductionDoneSync : public WorkData
	{
	public:
		WorkProductionDoneSync(WorkProducer *producer, int active_cores);
		virtual ~WorkProductionDoneSync();
		
		void worker_done()
		{
			if (done_jobs.increment() == active_cores)
				done_event.set(); //Only fire even once every worker is finished
		}

		bool is_done() const
		{
			return done_jobs.get() == active_cores;
		}

		CL_Event done_event;

		virtual void handle() { worker_done(); }
		virtual bool isHandled() { return handled; }
		virtual WorkProducer *getProducer() const { return producer; }

	private:
		int active_cores;
		CL_InterlockedVariable done_jobs;
		volatile bool handled;

		WorkProducer *producer;
	};
}
