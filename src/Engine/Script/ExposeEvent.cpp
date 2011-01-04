#include "precomp.h"
#include "ExposeEvent.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Entity/IEntity.h>

using namespace Engine;
using namespace LuaPlus;
using namespace Events;

ExposeEvent::ExposeEvent(CoreMgr *coreMgr, const Events::Event *event)
: event(event)
{
	this->coreMgr = coreMgr;
	init();
}

ExposeEvent::~ExposeEvent()
{
	lEvent.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeEvent::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	
	if(lEvents.IsNil())
		lEvents = globals.GetByName("Events");
	if(lEvents.IsNil())
		lEvents = globals.CreateTable("Events");

	lEvent = lEvents.CreateTable(event->getName().c_str());
	lEvent.SetString("id", event->getName().c_str());

	for(unsigned int i = 0; i < event->getArgumentCount(); i++)
	{
		Events::EventValue arg = event->getArgument(i);
		exposeArg(i, arg, lEvent);
	}
}

void ExposeEvent::exposeArg(unsigned int i, const Events::EventValue &arg, LuaPlus::LuaObject &lEvent)
{
	if(arg.IsBoolean())
	{
		exposeArg(i, arg.ToBoolean(), lEvent);
	}
	else if(arg.IsInteger())
	{
		exposeArg(i, arg.ToInteger(), lEvent);
	}
	else if(arg.IsUinteger())
	{
		exposeArg(i, (int)arg.ToUinteger(), lEvent); // Lua doesn't operate with unsigned...
	}
	else if(arg.IsNumber())
	{
		exposeArg(i, arg.ToNumber(), lEvent);
	}
	else if(arg.IsVec2())
	{
		exposeArg(i, arg.ToVec2(), lEvent);
	}
	else if(arg.IsVec3())
	{
		exposeArg(i, arg.ToVec3(), lEvent);
	}
	else if(arg.IsVec4())
	{
		exposeArg(i, arg.ToVec4(), lEvent);
	}
	else if(arg.IsString())
	{
		exposeArg(i, arg.ToString(), lEvent);
	}
	else if(arg.IsComponent())
	{
		exposeArg(i, *arg.ToComponent(), lEvent);
	}
	else if(arg.IsProperty())
	{
		exposeArg(i, *arg.ToProperty(), lEvent);
	}
	else if(arg.IsEntity())
	{
		exposeArg(i, *arg.ToEntity(), lEvent);
	}
}

void ExposeEvent::exposeArg(unsigned int i, const bool &arg, LuaPlus::LuaObject &lEvent)
{
	lEvent.SetBoolean(cl_format("arg%1", i).c_str(), arg);
}

void ExposeEvent::exposeArg(unsigned int i, const int &arg, LuaPlus::LuaObject &lEvent)
{
	lEvent.SetInteger(cl_format("arg%1", i).c_str(), arg);
}

void ExposeEvent::exposeArg(unsigned int i, const float &arg, LuaPlus::LuaObject &lEvent)
{
	lEvent.SetNumber(cl_format("arg%1", i).c_str(), arg);
}

void ExposeEvent::exposeArg(unsigned int i, const CL_Vec2f &arg, LuaPlus::LuaObject &lEvent)
{
	LuaObject lVec;
	lVec.SetNumber("x", arg.x);
	lVec.SetNumber("y", arg.y);
	lEvent.SetObject(cl_format("arg%1", i).c_str(), lVec);
}

void ExposeEvent::exposeArg(unsigned int i, const CL_Vec3f &arg, LuaPlus::LuaObject &lEvent)
{
	LuaObject lVec;
	lVec.SetNumber("x", arg.x);
	lVec.SetNumber("y", arg.y);
	lVec.SetNumber("z", arg.z);
	lEvent.SetObject(cl_format("arg%1", i).c_str(), lVec);
}

void ExposeEvent::exposeArg(unsigned int i, const CL_Vec4f &arg, LuaPlus::LuaObject &lEvent)
{
	LuaObject lVec;
	lVec.SetNumber("x", arg.x);
	lVec.SetNumber("y", arg.y);
	lVec.SetNumber("z", arg.z);
	lVec.SetNumber("w", arg.w);
	lEvent.SetObject(cl_format("arg%1", i).c_str(), lVec);
}

void ExposeEvent::exposeArg(unsigned int i, const CL_String &arg, LuaPlus::LuaObject &lEvent)
{
	lEvent.SetString(cl_format("arg%1", i).c_str(), arg.c_str());
}

void ExposeEvent::exposeArg(unsigned int i, const Component &arg, LuaPlus::LuaObject &lEvent)
{
}

void ExposeEvent::exposeArg(unsigned int i, const IProperty &arg, LuaPlus::LuaObject &lEvent)
{
}

void ExposeEvent::exposeArg(unsigned int i, const IEntity &arg, LuaPlus::LuaObject &lEvent)
{
}
