#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;
class IEntity;

class ExposeEntityManager;
class ExposePropertyContainer;
class ExposeComponentContainer;

class ExposeIEntity
{
public:
	ExposeIEntity(CoreMgr *coreMgr, ExposeEntityManager *exopsedEntityMgr, IEntity *entity);
	~ExposeIEntity();

	IEntity *getEntity() const { return entity; }
	LuaPlus::LuaObject getLEntity() const { return lEntity; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

protected:
	void init();

	void SendCommand(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lCommand);
	void SendEvent(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lEventType, LuaPlus::LuaObject lEventArg);
	LuaPlus::LuaObject HasComponent(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lName);

	CoreMgr *coreMgr;
	ExposeEntityManager *exposedEntityMgr;

	IEntity *entity;
	LuaPlus::LuaObject lEntity;
	LuaPlus::LuaObject lMeta;
	ExposePropertyContainer *exposedPropContainer;
	ExposeComponentContainer *exposedCompContainer;
};

}
