#pragma once

#include "Event.h"
#include <ClanLib/src/API/core.h>

namespace Engine
{
namespace Events
{

class IEventManager
{
public:
	typedef CL_Signal_v1<const Event &> CallbackClass;

	virtual CallbackClass &GetEvent(const CL_String &name) = 0;
	virtual void SendEvent(const Event &event) = 0;
};

}
}
