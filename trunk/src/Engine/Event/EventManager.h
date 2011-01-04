#pragma once

#include "IEventManager.h"
#include <map>

namespace Engine
{
namespace Events
{

class EventManager : public IEventManager
{
public:
	virtual CallbackClass &GetEvent(const CL_String &name);
	virtual void SendEvent(const Event &event);

private:
	std::map<CL_String, CallbackClass> eventHandlers;
};

}
}
