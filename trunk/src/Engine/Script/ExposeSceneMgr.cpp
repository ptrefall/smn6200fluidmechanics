#include "precomp.h"
#include "ExposeSceneMgr.h"
#include "ExposeIEntity.h"
#include "ExposeEntityManager.h"
#include <Core/CoreMgr.h>
#include <Entity/EntityManager.h>

#include <Event/IEventManager.h>
#include "ScriptMgr.h"
#include <Entity/IEntity.h>

using namespace Engine;
using namespace LuaPlus;

ExposeSceneMgr::ExposeSceneMgr(CoreMgr *coreMgr)
{
	this->coreMgr = coreMgr;
	init();
}

ExposeSceneMgr::~ExposeSceneMgr()
{
}

void ExposeSceneMgr::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("AddToScene", *this, &ExposeSceneMgr::AddToScene);
}

void ExposeSceneMgr::AddToScene(LuaPlus::LuaObject lEntity)
{
	if(!lEntity.IsTable())
	{
		CL_String msg = cl_format("Failed to add entity to scene, because the type of entity was %1 when expecting Table", lEntity.TypeName());
		throw CL_Exception(msg);
	}

	LuaObject lEntityId = lEntity.GetByName("id");
	if(!lEntityId.IsInteger())
	{
		CL_String msg = cl_format("EntityId was not an integer (it's a %1)", lEntityId.TypeName());
		throw CL_Exception(msg);
	}

	IEntity *entity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getExposedEntity((unsigned int)lEntityId.ToInteger())->getEntity();
	coreMgr->addToScene(entity);
}
