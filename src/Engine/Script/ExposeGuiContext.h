#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;

class ExposeGuiMgr;

class ExposeGuiContext
{
public:
	ExposeGuiContext(CoreMgr *coreMgr, ExposeGuiMgr *exopsedGuiMgr, Rocket::Core::Context *context);
	~ExposeGuiContext();

	Rocket::Core::Context *getContext() const { return context; }
	LuaPlus::LuaObject getLContext() const { return lContext; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

protected:
	void init();

	CoreMgr *coreMgr;
	ExposeGuiMgr *exposedGuiMgr;

	Rocket::Core::Context *context;
	LuaPlus::LuaObject lContext;
	LuaPlus::LuaObject lMeta;
};

}
