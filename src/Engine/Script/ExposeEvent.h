#pragma once

#include <LuaPlus/LuaPlus.h>
#include <Event/Event.h>
#include <Event/EventValue.h>

namespace Engine
{
class CoreMgr;
class ExposeEvent
{
public:
	ExposeEvent(CoreMgr *coreMgr, const Events::Event *event);
	~ExposeEvent();

	LuaPlus::LuaObject getLEvent() const { return lEvent; }
	LuaPlus::LuaObject getLEvents() const { return lEvents; }

private:
	void init();
	void exposeArg(unsigned int i, const Events::EventValue &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const bool &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const int &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const float &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const CL_Vec2f &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const CL_Vec3f &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const CL_Vec4f &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const CL_String &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const Component &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const IProperty &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const IEntity &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const Rocket::Core::Context &arg, LuaPlus::LuaObject &lEvent);
	void exposeArg(unsigned int i, const Rocket::Core::Element &arg, LuaPlus::LuaObject &lEvent);
	CoreMgr *coreMgr;

	const Events::Event *event;
	LuaPlus::LuaObject lEvent;
	LuaPlus::LuaObject lEvents;
};

}
