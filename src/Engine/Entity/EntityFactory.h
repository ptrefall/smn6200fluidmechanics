#pragma once

#include "IEntity.h"

namespace Engine
{
class IResource;
class CoreMgr;

class EntityFactory
{
public:
	EntityFactory(CoreMgr *coreMgr);
	~EntityFactory();

	void registerEntity(const CL_String &fileName);
	IEntity *create(const CL_String &type, const CL_String &name);
	void addDataAndLogic(IEntity *entity, const CL_String &type);

	static std::map<CL_String, CL_String>* creators;

	typedef IEntity*(* SpecialCreator)(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory);
	void RegisterSpecial(const CL_String &type, SpecialCreator functor);
	
	static std::map<CL_String, SpecialCreator>* special_creators;

private:
	IEntity* CreateSpecial(const CL_String &special, const CL_String &type, const CL_String &name);

	CL_String loadEntity(const CL_String &fileName);
	void loadComponents(IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported);
	void loadProperties(IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported);
	void loadProperties(const CL_String &entityType, const CL_DomNode &xmlNode, const CL_String &type);
	int addComponents(IEntity *entity, const CL_String &type);
	int addProperties(IEntity *entity, const CL_String &type);
	
	CoreMgr *coreMgr;

	std::map<CL_String, std::vector<CL_String>> entity_components;
	std::map<CL_String, std::vector<std::pair<CL_String, CL_String>>> entity_properties;
	std::map<CL_String, CL_String> entity_specials;
};

}
