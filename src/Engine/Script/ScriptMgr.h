#pragma once

#include <LuaPlus/LuaLink.h>
#include <LuaPlus/LuaPlus.h>
#include <LuaPlus/LuaObject.h>

namespace Engine
{
	class CoreMgr;

	class ExposeEntityManager;
	class ExposeComponentManager;
	class ExposeSceneMgr;
	class ExposeGuiMgr;

	class ScriptMgr
	{
	public:
		ScriptMgr(CoreMgr *coreMgr);
		~ScriptMgr();

		void init();

		LuaPlus::LuaStateOwner *GetGlobalState() { return &globalState; }

		int doFile(const CL_String &fileName);
		int doString(const CL_String &script, bool throwException = false, bool logError = true);
		CL_String getErrorOnStack();

		ExposeEntityManager *getExposedEntityMgr() const { return exposedEntityMgr; }
		ExposeComponentManager *getExposedComponentMgr() const { return exposedComponentMgr; }
		ExposeSceneMgr *getExposedSceneMgr() const { return exposedSceneMgr; }
		ExposeGuiMgr *getExposedGuiMgr() const { return exposedGuiMgr; }

	private:
		void Print(LuaPlus::LuaObject text);

		LuaPlus::LuaStateOwner globalState;
		LuaPlus::LuaObject metaTable;

		CoreMgr *coreMgr;

		ExposeEntityManager *exposedEntityMgr;
		ExposeComponentManager *exposedComponentMgr;
		ExposeSceneMgr *exposedSceneMgr;
		ExposeGuiMgr *exposedGuiMgr;
	};
}
