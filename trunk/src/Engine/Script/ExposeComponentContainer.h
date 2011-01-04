#pragma once

#include <LuaPlus/LuaPlus.h>
#include <vector>
#include <Event/EventContainer.h>
#include <Event/Event.h>

namespace Engine
{
class CoreMgr;

class ExposeIEntity;
class ExposeComponent;

class ExposeComponentContainer
{
public:
	ExposeComponentContainer(CoreMgr *coreMgr, ExposeIEntity *exposedEntity);
	~ExposeComponentContainer();

	LuaPlus::LuaObject &getLComps() { return lComponents; }

private:
	void init();

	void AddComponent(LuaPlus::LuaObject self, LuaPlus::LuaObject lName);

	CoreMgr *coreMgr;
	ExposeIEntity *exposedEntity;

	LuaPlus::LuaObject lComponents;
	std::vector<ExposeComponent*> exposedComponents;

	void OnComponentAdded(const Events::Event &event);
	Events::EventContainer engineEvents;
};

}
