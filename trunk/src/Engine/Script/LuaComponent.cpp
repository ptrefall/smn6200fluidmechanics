#include "precomp.h"
#include "LuaComponent.h"
#include "ExposeComponent.h"
#include "ExposeEntityManager.h"
#include "ExposeEvent.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>

#include <Entity/ComponentContainer.h>
#include <Entity/PropertyContainer.h>
#include <Event/Event.h>
#include <Event/CommandHandler.h>

#include <LuaPlus/LuaPlus.h>

using namespace Engine;
using namespace LuaPlus;

LuaComponent::LuaComponent(CoreMgr *coreMgr, IEntity *entity, const CL_String &name)
: Component(entity, name), coreMgr(coreMgr)
{
	exposedComp = NULL;
	hasInit = false;
	hasUpdate = false;
	hasCommand = false;
	hasEvent = false;
}

void LuaComponent::initLuaExposure(ExposeComponent *exposedComp)
{
	this->exposedComp = exposedComp;
	
	hasInit = false;
	LuaObject lInit = exposedComp->getLComp().GetByName("OnInit");
	if(lInit.IsFunction())
	{
		hasInit = true;
	}

	hasUpdate = false;
	LuaObject lUpdate = exposedComp->getLComp().GetByName("OnUpdate");
	if(lUpdate.IsFunction())
	{
		hasUpdate = true;
	}
	
	hasCommand = false;
	LuaObject lCommand = exposedComp->getLComp().GetByName("OnCommand");
	if(lCommand.IsFunction())
	{
		hasCommand = true;
	}
	
	hasEvent = false;
	LuaObject lEvent = exposedComp->getLComp().GetByName("OnEvent");
	if(lEvent.IsFunction())
	{
		hasEvent = true;
	}

	if(hasInit)
	{
		//Make sure that the entity exist in the global state's Entities table
		LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntities().GetByIndex(entity->getId());
		if(lEntity.IsNil())
		{
			CL_String err = cl_format("Entity of id %1 has not been exposed to Lua!", entity->getId());
			throw CL_Exception(err);
		}

		coreMgr->getScriptMgr()->doString(cl_format("%1:OnInit(Entities[%2])", name, entity->getId()));
	}
}

void LuaComponent::ExecuteCommand(const CL_String &command)
{
	if(hasCommand)
	{
		//Make sure that the entity exist in the global state's Entities table
		LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntities().GetByIndex(entity->getId());
		if(lEntity.IsNil())
		{
			CL_String err = cl_format("Entity of id %1 has not been exposed to Lua!", entity->getId());
			throw CL_Exception(err);
		}

		coreMgr->getScriptMgr()->doString(cl_format("%1:OnCommand(Entities[%2], '%3')", name, entity->getId(), command));
	}
}

void LuaComponent::ExecuteEvent(const Events::Event &event)
{
	if(hasEvent)
	{
		//Make sure that the entity exist in the global state's Entities table
		LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntities().GetByIndex(entity->getId());
		if(lEntity.IsNil())
		{
			CL_String err = cl_format("Entity of id %1 has not been exposed to Lua!", entity->getId());
			throw CL_Exception(err);
		}

		ExposeEvent exposedEvent(coreMgr, &event);

		//Make sure that the event exist in the global state's Events table
		LuaObject lEvent = exposedEvent.getLEvents().GetByName(event.getName().c_str());
		if(lEvent.IsNil())
		{
			CL_String err = cl_format("Event of type %1 has not been exposed to Lua!", event.getName().c_str());
			throw CL_Exception(err);
		}
		coreMgr->getScriptMgr()->doString(cl_format("%1:OnEvent(Entities[%2], Events['%3'])", name, entity->getId(), event.getName().c_str()));
	}
}

void LuaComponent::Update(double dt)
{
	if(hasUpdate)
	{
		//Make sure that the entity exist in the global state's Entities table
		LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntities().GetByIndex(entity->getId());
		if(lEntity.IsNil())
		{
			CL_String err = cl_format("Entity of id %1 has not been exposed to Lua!", entity->getId());
			throw CL_Exception(err);
		}

		coreMgr->getScriptMgr()->doString(cl_format("%1:OnUpdate(Entities[%2], %3)", name, entity->getId(), dt));
	}
}
