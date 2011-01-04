#pragma once

#include "EventValue.h"
#include <ClanLib/src/API/core.h>

namespace Engine
{
namespace Events
{

class Event
{
public:
	Event(const CL_String &name);
	Event(const CL_String &name, const EventValue &arg1);
	Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2);
	Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2, const EventValue &arg3);
	Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2, const EventValue &arg3, const EventValue &arg4);
	Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2, const EventValue &arg3, const EventValue &arg4, const EventValue &arg5);

	const CL_String &getName() const { return name; };

	unsigned int getArgumentCount() const;

	EventValue getArgument(unsigned int index) const;

	void addArgument(const EventValue &value);

	CL_String toString() const;

private:
	CL_String toString(const EventValue &v) const;

	CL_String name;
	std::vector<EventValue> arguments;
};

}
}
