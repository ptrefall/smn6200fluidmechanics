#include "precomp.h"
#include "ExposeComponentManager.h"
#include "ExposeComponent.h"
#include "LuaComponent.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>

#include <Entity/EntityManager.h>
#include <Entity/IEntity.h>
#include <Entity/Component.h>
#include <Resource/ResMgr.h>

using namespace Engine;
using namespace LuaPlus;

ExposeComponentManager::ExposeComponentManager(CoreMgr *coreMgr)
{
	this->coreMgr = coreMgr;
	init();
}

ExposeComponentManager::~ExposeComponentManager()
{
}

void ExposeComponentManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("RegisterComponent", *this, &ExposeComponentManager::RegisterComponent);

	//Load all scripts in Scripts/Components
	std::vector<CL_String> scripts = coreMgr->getResMgr()->getFilesInDir("/Scripts/Components/");
	for(unsigned int i = 0; i < scripts.size(); i++)
	{
		int fail = coreMgr->getScriptMgr()->doFile(cl_format("Components/%1", scripts[i]));
		if(fail)
		{
			CL_String err = cl_format("Failed to load component script %1", scripts[i]);
			throw CL_Exception(err);
		}
	}
}

void ExposeComponentManager::RegisterComponent(LuaObject lName)
{
	if(!lName.IsString())
	{
		CL_String err = cl_format("Failed to register component, because the type of name was %1 when expecting String!", lName.TypeName());
		throw CL_Exception(err);
	}

	CL_String name = lName.ToString();
	coreMgr->getEntityMgr()->getComponentFactory()->RegisterComponent(name, &LuaComponent::Create);
}
