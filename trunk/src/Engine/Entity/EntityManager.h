#pragma once

namespace Engine
{
class CoreMgr;
class IEntity;
class EntityFactory;
class ComponentFactory;
class EntityManager
{
public:
	EntityManager(CoreMgr *coreMgr);
	~EntityManager();

	ComponentFactory *getComponentFactory() const { return compFactory; }
	IEntity *create(const CL_String &type, const CL_String &name = CL_String());

	void update(float dt);

	unsigned int genUId()
	{
		uIds++;
		return uIds;
	}

private:
	void init();

	CoreMgr *coreMgr;
	EntityFactory *factory;
	ComponentFactory *compFactory;
	unsigned int uIds;

	std::vector<IEntity*> entities;
};

}
