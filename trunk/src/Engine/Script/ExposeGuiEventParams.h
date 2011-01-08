#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;
class ExposeEvent;

class ExposeGuiEventParams
{
public:
	ExposeGuiEventParams(CoreMgr *coreMgr, ExposeEvent &e_event, Rocket::Core::Event& event);
	~ExposeGuiEventParams();

	LuaPlus::LuaObject &getLParams() { return lParams; }
	LuaPlus::LuaObject getLParam(const CL_String &name);

private:
	void init();

	CoreMgr *coreMgr;
	ExposeEvent &e_event;
	Rocket::Core::Event& event;
	LuaPlus::LuaObject lParams;
};

}
