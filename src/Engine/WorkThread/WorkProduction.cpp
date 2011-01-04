#include "precomp.h"
#include "WorkProduction.h"
#include "WorkDoneData.h"

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

WorkProduction::WorkProduction(WorkProducer *producer, const std::vector<WorkData*> &work, WorkDoneData *doneData)
: done(false), local_num_finished(0)
{
	this->producer = producer;
	this->work = work;
	this->doneData = doneData;

	finished_work.resize(work.size(), false);

	compiler_barrier();
	num_work.set(work.size());
}

WorkProduction::~WorkProduction()
{
}
