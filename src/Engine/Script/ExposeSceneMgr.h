#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;

class ExposeSceneMgr
{
public:
	ExposeSceneMgr(CoreMgr *coreMgr);
	~ExposeSceneMgr();

private:
	void init();

	void AddToScene(LuaPlus::LuaObject lEntity);

	CoreMgr *coreMgr;
};

}
