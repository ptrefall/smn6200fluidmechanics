#include "precomp.h"
#include "WorkProductionDoneSync.h"

using namespace Engine;

WorkProductionDoneSync::WorkProductionDoneSync(WorkProducer *producer, int active_cores)
: producer(producer), active_cores(active_cores), handled(false)
{
}

WorkProductionDoneSync::~WorkProductionDoneSync()
{
}
