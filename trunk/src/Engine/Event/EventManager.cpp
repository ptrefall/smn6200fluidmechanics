#include "precomp.h"
#include "EventManager.h"

using namespace Engine;
using namespace Events;

EventManager::CallbackClass &EventManager::GetEvent(const CL_String &name)
{
	return eventHandlers[name];
}

void EventManager::SendEvent(const Event &event)
{
	std::map<CL_String, CallbackClass>::iterator it;
	it = eventHandlers.find(event.getName());
	if(it != eventHandlers.end())
	{
		it->second.invoke(event);
		//cl_log_event("Event", "Published event: " + event.toString());
	}
	else
	{
		//cl_log_event("Event", "Published event (no subscribers): " + event.toString());
	}
}
