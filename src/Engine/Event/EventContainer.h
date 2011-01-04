#pragma once

#include "IEventManager.h"
#include <ClanLib/src/API/core.h>
#include <vector>

namespace Engine
{
namespace Events
{

class EventContainer
{
public:
	EventContainer(IEventManager *EventMgr) : EventMgr(EventMgr) {}
	~EventContainer() {}

	template<class Class, class Param1>
	void Connect(const CL_String &eventName, Class *self, void(Class::*func)(Param1))
	{
		slots.push_back(EventMgr->GetEvent(eventName).connect(self, func));
	}

	template<class Class, class Param1, class UserData>
	void Connect(const CL_String &eventName, Class *self, void(Class::*func)(Param1, UserData), UserData userData)
	{
		slots.push_back(EventMgr->GetEvent(eventName).connect(self, func, userData));
	}

private:
	std::vector<CL_Slot> slots;
	IEventManager *EventMgr;
};

}
}
