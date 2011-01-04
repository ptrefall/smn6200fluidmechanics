#include "precomp.h"
#include "Event.h"

using namespace Engine;
using namespace Events;

Event::Event(const CL_String &name)
: name(name)
{
}

Event::Event(const CL_String &name, const EventValue &arg1)
: name(name)
{
	addArgument(arg1);
}

Event::Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
}

Event::Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2, const EventValue &arg3)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
	addArgument(arg3);
}

Event::Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2, const EventValue &arg3, const EventValue &arg4)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
	addArgument(arg3);
	addArgument(arg4);
}

Event::Event(const CL_String &name, const EventValue &arg1, const EventValue &arg2, const EventValue &arg3, const EventValue &arg4, const EventValue &arg5)
: name(name)
{
	addArgument(arg1);
	addArgument(arg2);
	addArgument(arg3);
	addArgument(arg4);
	addArgument(arg5);
}

unsigned int Event::getArgumentCount() const
{
	return arguments.size();
}

EventValue Event::getArgument(unsigned int index) const
{
	if (index >= arguments.size())
		throw CL_Exception(cl_format("Arguments out of bounds for engine event %1", name));
	return arguments[index];
}

void Event::addArgument(const EventValue &value)
{
	arguments.push_back(value);
}

CL_String Event::toString() const
{
	CL_String eventInfo = cl_format("%1(", name);

	for(unsigned int i = 0; i < arguments.size(); ++i)
	{
		if(i > 0)
			eventInfo += ",";

		eventInfo += toString(arguments[i]);
	}

	eventInfo += ")";

	return eventInfo;
}

CL_String Event::toString(const EventValue &v) const
{
	switch (v.GetType())
	{
	case EventValue::null:
		return "null";
	case EventValue::integer:
		return CL_StringHelp::int_to_text(v.ToInteger());
	case EventValue::uinteger:
		return CL_StringHelp::uint_to_text(v.ToUinteger());
	case EventValue::string:
		return "\"" + v.ToString() + "\"";
	case EventValue::boolean:
		return v.ToBoolean() ? "true" : "false";
	case EventValue::number:
		return CL_StringHelp::float_to_text(v.ToNumber());
	default:
		return "??" + CL_StringHelp::int_to_text(v.GetType());
	}
}
