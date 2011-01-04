#include "precomp.h"
#include "ExposeEntityManager.h"
#include "ExposeIEntity.h"
#include <Core/CoreMgr.h>
#include <Entity/EntityManager.h>

#include <Event/IEventManager.h>
#include "ScriptMgr.h"
#include <Entity/IEntity.h>

using namespace Engine;
using namespace LuaPlus;

ExposeEntityManager::ExposeEntityManager(CoreMgr *coreMgr)
: engineEvents(coreMgr->getEventMgr())
{
	this->coreMgr = coreMgr;
	init();
}

ExposeEntityManager::~ExposeEntityManager()
{
}

void ExposeEntityManager::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("CreateEntity", *this, &ExposeEntityManager::CreateEntity);
	lEntities = globals.CreateTable("Entities");

	engineEvents.Connect("EntityCreated", this, &ExposeEntityManager::OnEntityCreated);
	engineEvents.Connect("EntityRemoved", this, &ExposeEntityManager::OnEntityRemoved);
}

LuaObject ExposeEntityManager::getLEntity(unsigned int id)
{
	return lEntities.GetByIndex((int)id);
}

ExposeIEntity *ExposeEntityManager::getExposedEntity(unsigned int id)
{
	for(unsigned int i = 0; i < exposedEntities.size(); i++)
	{
		if(exposedEntities[i]->getEntity()->getId() == id)
		{
			return exposedEntities[i];
		}
	}
	return NULL;
}

LuaObject ExposeEntityManager::CreateEntity(LuaPlus::LuaObject lType, LuaPlus::LuaObject lName)
{
	if(!lType.IsString())
	{
		CL_String msg = cl_format("Failed to create entity, because the type of type was %1 when expecting String", lType.TypeName());
		throw CL_Exception(msg);
	}
	if(!lName.IsString() && !lName.IsNil())
	{
		CL_String msg = cl_format("Failed to create entity, because the type of name was %1 when expecting String or Nil", lName.TypeName());
		throw CL_Exception(msg);
	}

	CL_String type = lType.ToString();
	CL_String name;

	IEntity *entity = NULL;
	if(lName.IsString())
	{
		name = lName.ToString();
		if(name != CL_String())
		{
			entity = coreMgr->getEntityMgr()->create(type, name);
		}
		else
		{
			entity = coreMgr->getEntityMgr()->create(type);
		}
	}
	else
	{
		entity = coreMgr->getEntityMgr()->create(type);
	}

	if(entity == NULL)
	{
		CL_String msg = cl_format("Failed to create entity %1 of type %2", name, type);
		throw CL_Exception(msg);
	}

	ExposeIEntity *exposedEntity = getExposedEntity(entity->getId());

	if(exposedEntity)
		return exposedEntity->getLEntity();
	else
		return LuaObject(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeEntityManager::OnEntityCreated(const Events::Event &event)
{
	IEntity *entity = event.getArgument(0).ToEntity();

	ExposeIEntity *exposedEntity = new ExposeIEntity(coreMgr, this, entity);
	exposedEntities.push_back(exposedEntity);
}

void ExposeEntityManager::OnEntityRemoved(const Events::Event &event)
{
	IEntity *entity = event.getArgument(0).ToEntity();
	for(unsigned int i = 0; i < exposedEntities.size(); i++)
	{
		if(exposedEntities[i]->getEntity()->getId() == entity->getId())
		{
			ExposeIEntity *exposedEntity = exposedEntities[i];
			exposedEntities.erase(exposedEntities.begin() + i);
			delete exposedEntity;
			exposedEntity = NULL;
			break;
		}
	}
}
