#include "precomp.h"
#include "ExposeComponentContainer.h"
#include "ExposeComponent.h"
#include "ExposeIEntity.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>

#include <Entity/IEntity.h>
#include <Entity/Component.h>
#include <Event/IEventManager.h>
#include <Event/Event.h>

using namespace Engine;
using namespace LuaPlus;

ExposeComponentContainer::ExposeComponentContainer(CoreMgr *coreMgr, ExposeIEntity *exposedEntity)
: engineEvents(coreMgr->getEventMgr())
{
	this->coreMgr = coreMgr;
	this->exposedEntity = exposedEntity;
	init();
}

ExposeComponentContainer::~ExposeComponentContainer()
{
	for(unsigned int i = 0; i < exposedComponents.size(); i++)
	{
		ExposeComponent *exposedComp = exposedComponents[i];
		delete exposedComp;
		exposedComp = NULL;
	}
	exposedComponents.clear();
	lComponents.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeComponentContainer::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	lComponents = exposedEntity->getLEntity().CreateTable("Components");

	{
		LuaObject lMeta = lComponents.CreateTable("MetaTable");
		lMeta.SetObject("__index", lMeta);
		lMeta.RegisterDirect("Add", *this, &ExposeComponentContainer::AddComponent);
		lComponents.SetLightUserData("__object", this);
		lComponents.SetMetaTable(lMeta);
	}

	{
		LuaObject lMeta = exposedEntity->getLMeta();
		lMeta.RegisterDirect("AddComponent", *this, &ExposeComponentContainer::AddComponent);
	}
	
	IEntity *entity = exposedEntity->getEntity();
	std::vector<Component*> &components = entity->GetComponents();
	std::vector<Component*>::iterator compIt = components.begin();
	for(; compIt != components.end(); ++compIt)
	{
		ExposeComponent *exposedComp = new ExposeComponent(coreMgr, exposedEntity, this, (*compIt));
		exposedComponents.push_back(exposedComp);
	}

	engineEvents.Connect("ComponentAdded", this, &ExposeComponentContainer::OnComponentAdded);
}

void ExposeComponentContainer::AddComponent(LuaObject self, LuaObject lName)
{
	if(!self.IsTable())
	{
		CL_String self_type = self.TypeName();
		
		CL_String err = cl_format("Failed to add component, because the type of self was %1 when expecting Table", self_type);
		throw CL_Exception(err);
	}

	if(!lName.IsString())
	{
		CL_String name_type = lName.TypeName();
		
		CL_String err = cl_format("Failed to add component, because the type of name was %1 when expecting String", name_type);
		throw CL_Exception(err);
	}

	CL_String name = lName.ToString();
	
	if(exposedEntity == NULL)
	{
		CL_String err = cl_format("Failed to add component %1, because there is no entity!", name);
		throw CL_Exception(err);
	}

	Component *comp = exposedEntity->getEntity()->AddComponent(name);
	if(comp == NULL)
	{
		CL_String err = cl_format("Failed to add component %1, because no component was returned!", name);
		throw CL_Exception(err);
	}
}

void ExposeComponentContainer::OnComponentAdded(const Events::Event &event)
{
	IEntity *entity = event.getArgument(1).ToEntity();
	if(entity->getId() != exposedEntity->getEntity()->getId())
		return;

	Component *comp = event.getArgument(0).ToComponent();

	ExposeComponent *exposedComp = new ExposeComponent(coreMgr, exposedEntity, this, comp);
	exposedComponents.push_back(exposedComp);
}
