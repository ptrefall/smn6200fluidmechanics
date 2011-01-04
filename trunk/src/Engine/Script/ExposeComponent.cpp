#include "precomp.h"
#include "ExposeComponent.h"
#include "ExposeIEntity.h"
#include "ExposeComponentContainer.h"
#include "LuaComponent.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Entity/Component.h>
#include <Entity/IEntity.h>
#include <Event/Event.h>

using namespace Engine;
using namespace LuaPlus;

ExposeComponent::ExposeComponent(CoreMgr *coreMgr, ExposeIEntity *exposedEntity, ExposeComponentContainer *exposedCompContainer, Component *component)
{
	this->coreMgr = coreMgr;
	this->exposedEntity = exposedEntity;
	this->exposedCompContainer = exposedCompContainer;
	this->component = component;
	init();
}

ExposeComponent::~ExposeComponent()
{
	lComponent.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeComponent::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	
	//Check if this is a C++ component, or a scripted Lua defined component
	LuaComponent *luaComp = dynamic_cast<LuaComponent*>(component);
	if(luaComp == NULL)
	{
		LuaObject &lComps = exposedCompContainer->getLComps();
		lComponent = lComps.CreateTable(component->GetName().c_str());
		lComponent.SetString("id", component->GetName().c_str());
		
		{
			LuaObject lMeta = lComponent.CreateTable("MetaTable");
			lMeta.SetObject("__index", lMeta);
			
			lComponent.SetLightUserData("__object", this);
			lComponent.SetMetaTable(lMeta);
		}
	}
	else
	{
		lComponent = globals.GetByName(component->GetName().c_str());
		lComponent.SetString("id", component->GetName().c_str());
		{
			LuaObject lMeta = lComponent.CreateTable("MetaTable");
			lMeta.SetObject("__index", lMeta);
			
			lComponent.SetLightUserData("__object", this);
			lComponent.SetMetaTable(lMeta);
		}

		luaComp->initLuaExposure(this);
	}
}
