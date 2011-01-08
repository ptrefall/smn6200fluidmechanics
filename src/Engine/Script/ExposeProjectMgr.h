#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;

class ExposeProjectMgr
{
public:
	ExposeProjectMgr(CoreMgr *coreMgr);
	~ExposeProjectMgr();

private:
	void init();

	LuaPlus::LuaObject CreateProject(LuaPlus::LuaObject lFilename);
	LuaPlus::LuaObject LoadProject(LuaPlus::LuaObject lFilename);

	CoreMgr *coreMgr;
};

}
