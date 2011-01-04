#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;
class Component;

class ExposeIEntity;
class ExposeComponentContainer;

class ExposeComponent
{
public:
	ExposeComponent(CoreMgr *coreMgr, ExposeIEntity *exposedEntity, ExposeComponentContainer *exposedCompContainer, Component *component);
	~ExposeComponent();

	Component *getComp() const { return component; }
	LuaPlus::LuaObject getLComp() const { return lComponent; }
	ExposeIEntity *getExposedEntity() const { return exposedEntity; }

private:
	void init();

	CoreMgr *coreMgr;
	ExposeComponentContainer *exposedCompContainer;
	ExposeIEntity *exposedEntity;

	Component *component;
	LuaPlus::LuaObject lComponent;
};

}
