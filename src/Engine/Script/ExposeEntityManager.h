#pragma once

#include <LuaPlus/LuaPlus.h>
#include <Event/EventContainer.h>
#include <Event/Event.h>

namespace Engine
{
class CoreMgr;
class ExposeIEntity;

class ExposeEntityManager
{
public:
	ExposeEntityManager(CoreMgr *coreMgr);
	~ExposeEntityManager();

	LuaPlus::LuaObject &getLEntities() { return lEntities; }
	LuaPlus::LuaObject getLEntity(unsigned int id);
	ExposeIEntity *getExposedEntity(unsigned int id);

private:
	void init();

	LuaPlus::LuaObject CreateEntity(LuaPlus::LuaObject lType, LuaPlus::LuaObject lName);

	CoreMgr *coreMgr;

	LuaPlus::LuaObject lEntities;
	std::vector<ExposeIEntity*> exposedEntities;

	void OnEntityCreated(const Events::Event &event);
	void OnEntityRemoved(const Events::Event &event);
	Events::EventContainer engineEvents;
};

}
