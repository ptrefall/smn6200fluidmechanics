#pragma once

namespace Engine
{
	class WorkData;
	class WorkDoneData;
	class WorkProducer;
	class WorkProduction
	{
	public:
		WorkProduction(WorkProducer *producer, const std::vector<WorkData*> &work, WorkDoneData *doneData);
		virtual ~WorkProduction();

		bool isDone() const { return done; }
		WorkDoneData * getDoneData() const { return doneData; }
		WorkData *getWorkData(unsigned int i) { return work[i]; }
		unsigned int getWorkDataSize() const { return work.size(); }
		unsigned int getNumFinished() const { return num_finished.get(); }

	private:
		std::vector<WorkData*> work;
		std::vector<bool> finished_work;
		
		WorkDoneData *doneData;
		WorkProducer *producer;

		volatile unsigned int local_num_finished;
		CL_InterlockedVariable num_finished;
		CL_InterlockedVariable num_work;
		volatile bool done;
		
	};
}
