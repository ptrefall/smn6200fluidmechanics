#pragma once

#include <LuaPlus/LuaPlus.h>

#include <Entity/Property.h>
#include <Entity/IEntity.h>
#include <Event/EventContainer.h>
#include <Event/Event.h>

namespace Engine
{
class CoreMgr;
class ExposeIEntity;
class ExposeIProperty;

class ExposePropertyContainer
{
public:
	ExposePropertyContainer(CoreMgr *coreMgr, ExposeIEntity *exposedEntity);
	~ExposePropertyContainer();

	LuaPlus::LuaObject &getLProps() { return lProperties; }

private:
	void init();

	void AddProperty(LuaPlus::LuaObject self, LuaPlus::LuaObject name, LuaPlus::LuaObject defValue);
	LuaPlus::LuaObject HasProperty(LuaPlus::LuaObject self, LuaPlus::LuaObject name);

	void add(const CL_String &name, IProperty *prop, ExposeIProperty *exposedProp);

	CoreMgr *coreMgr;
	ExposeIEntity *exposedEntity;

	LuaPlus::LuaObject lProperties;
	std::vector<ExposeIProperty*> exposedProperties;

	void OnPropertyAdded(const Events::Event &event);
	Events::EventContainer engineEvents;
};

}
