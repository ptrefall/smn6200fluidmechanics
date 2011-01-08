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

	void CreateProject(LuaPlus::LuaObject lFilename);

	CoreMgr *coreMgr;
};

}
