#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;

class ExposeComponentManager
{
public:
	ExposeComponentManager(CoreMgr *coreMgr);
	~ExposeComponentManager();

private:
	void init();

	void RegisterComponent(LuaPlus::LuaObject lName);

	CoreMgr *coreMgr;
};

}
