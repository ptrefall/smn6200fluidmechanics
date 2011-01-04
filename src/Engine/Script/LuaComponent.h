#pragma once

#include <Entity/Component.h>
#include <Event/Event.h>
#include <Entity/IEntity.h>

namespace Engine
{
class CoreMgr;
class ExposeComponent;

class LuaComponent : public Component
{
public:
	virtual ~LuaComponent() {}

	virtual void Update(double dt);
	virtual void ExecuteCommand(const CL_String &command);
	virtual void ExecuteEvent(const Events::Event &event);

	static CL_String GetType() { return "Lua"; }
	static Component* Create(CoreMgr *coreMgr, IEntity *entity, const CL_String &name) { return new LuaComponent(coreMgr, entity, name); }

	void initLuaExposure(ExposeComponent *exposedComp);

protected:
	LuaComponent(CoreMgr *coreMgr, IEntity *entity, const CL_String &name);

	CoreMgr *coreMgr;
	ExposeComponent *exposedComp;

	bool hasInit, hasUpdate, hasCommand, hasEvent;
};
}
