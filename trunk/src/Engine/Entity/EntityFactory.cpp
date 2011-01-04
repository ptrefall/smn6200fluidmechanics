#include "precomp.h"
#include "EntityFactory.h"
#include "EntityManager.h"

#include <Core/CoreMgr.h>

#include <Resource/ResMgr.h>
#include <Resource/IResource.h>
#include <Resource/XMLResource.h>

#include "TypeSerializer.h"

using namespace Engine;

std::map<CL_String, CL_String>* EntityFactory::creators;
std::map<CL_String, EntityFactory::SpecialCreator>* EntityFactory::special_creators;

EntityFactory::EntityFactory(CoreMgr *coreMgr)
{
	this->coreMgr = coreMgr;
}

EntityFactory::~EntityFactory()
{
	if(creators)
	{
		creators->clear();
		delete creators;
		creators = NULL;
	}
	if(special_creators)
	{
		special_creators->clear();
		delete special_creators;
		special_creators = NULL;
	}
}

void EntityFactory::RegisterSpecial(const CL_String &type, SpecialCreator functor)
{
	if(special_creators == 0)
		special_creators = new std::map<CL_String, SpecialCreator>();

	if(special_creators->find(type) == special_creators->end())
	{
		std::pair<CL_String, SpecialCreator> value(type, functor);
		special_creators->insert(value);
	}
}

IEntity* EntityFactory::CreateSpecial(const CL_String &special, const CL_String &type, const CL_String &name)
{
	if(special_creators == 0)
		throw CL_Exception("Special Creators has not been instanciated!");

	std::map<CL_String, SpecialCreator>::iterator creatorIt = special_creators->find(special);
	if(creatorIt == special_creators->end())
		throw CL_Exception("Unable to create special entity " + special);
	
	SpecialCreator creator = creatorIt->second;
	return creator(coreMgr->getEntityMgr()->genUId(), type, name, coreMgr, *coreMgr->getEntityMgr()->getComponentFactory());
}

void EntityFactory::registerEntity(const CL_String &fileName)
{
	if(creators == 0)
		creators = new std::map<CL_String, CL_String>();

	//Check if file has already been loaded, though fileName is registered as
	//second value in the map, because accessing the first value is faster, and
	//we need that speed in the run-time create() function more than we need that
	//speed at register initialization.
	std::map<CL_String, CL_String>::iterator it = creators->begin();
	for(; it != creators->end(); ++it)
	{
		if((*it).second == fileName)
		{
			return;
		}
	}

	CL_String type = loadEntity(fileName);

	std::pair<CL_String, CL_String> value(type, fileName);
	creators->insert(value);
}

IEntity *EntityFactory::create(const CL_String &type, const CL_String &name)
{
	if(creators == 0)
		throw CL_Exception("EntityCreator map has not been instanciated!");

	std::map<CL_String, CL_String>::iterator creatorIt = creators->find(type);
	if(creatorIt == creators->end())
		throw CL_Exception(cl_format("%1 %2", "Unable to create entity of type", type));
	
	IEntity *entity = NULL;
	
	std::map<CL_String, CL_String>::iterator specialIt = entity_specials.find(type);
	if(specialIt != entity_specials.end())
	{
		CL_String specialType = specialIt->second;
		entity = CreateSpecial(specialType, type, name);
	}
	else
	{
		entity = new IEntity(coreMgr->getEntityMgr()->genUId(), type, name, coreMgr, *coreMgr->getEntityMgr()->getComponentFactory());
	}

	return entity;
}

void EntityFactory::addDataAndLogic(IEntity *entity, const CL_String &type)
{
	int fail = addComponents(entity, type);
	if(fail)
		throw CL_Exception("Failed to add components");

	fail = addProperties(entity, type);
	if(fail)
		throw CL_Exception("Failed to set properties");
}

int EntityFactory::addComponents(IEntity *entity, const CL_String &type)
{
	std::map<CL_String, std::vector<CL_String>>::iterator compIt = entity_components.find(type);
	if(compIt == entity_components.end())
	{
		return 1;
	}

	std::vector<CL_String> compTypes = compIt->second;
	for(unsigned int i = 0; i < compTypes.size(); i++)
	{
		try
		{
			entity->AddComponent(compTypes[i]);
		}
		catch(const CL_Exception &e)
		{
			CL_String err = cl_format("Failed to add component to Entity of type %1: %2", type, e.what());
			throw CL_Exception(err);
		}
	}

	return 0;
}

int EntityFactory::addProperties(IEntity *entity, const CL_String &type)
{
	std::map<CL_String, std::vector<std::pair<CL_String, CL_String>>>::iterator propIt = entity_properties.find(type);
	if(propIt == entity_properties.end())
	{
		return 1;
	}

	std::vector<std::pair<CL_String, CL_String>> propTypes = propIt->second;
	for(unsigned int i = 0; i < propTypes.size(); i++)
	{
		CL_String name = propTypes[i].first;
		if(!entity->HasProperty(name))
			continue;

		CL_String value = propTypes[i].second;
		IProperty *propInterface = entity->GetIProperty(name);
		propInterface->SetFromString(value);
	}

	return 0;
}

CL_String EntityFactory::loadEntity(const CL_String &fileName)
{
	IResource *res = coreMgr->getResMgr()->create(cl_format("%1/%2", "Entities", fileName), "XML");

	CL_String type = res->getString("Entity/Type");
	CL_String inherit;
	try
	{
		inherit = res->getString("Entity/Inherits");
	}
	catch(const CL_Exception &)
	{
		inherit = CL_String();
	}

	CL_String special;
	try
	{
		special = res->getString("Entity/Special");
	}
	catch(const CL_Exception &)
	{
		special = CL_String();
	}

	//If this object inherits from another, make sure that the
	//parent object is already loaded, if not, load it before
	//we continue
	bool inheritSupported = false;
	if(inherit != CL_String())
	{
		std::map<CL_String, CL_String>::iterator creatorIt = creators->find(inherit);
		if(creatorIt == creators->end())
		{
			registerEntity(cl_format("%1%2", inherit, ".xml"));
			creatorIt = creators->find(inherit);
			if(creatorIt != creators->end())
			{
				inheritSupported = true;
			}
		}
		else
		{
			inheritSupported = true;
		}
	}

	//Check for special entity type
	if(special != CL_String())
	{
		entity_specials[type] = special;
	}

	loadComponents(res, type, inherit, inheritSupported);
	loadProperties(res, type, inherit, inheritSupported);
	return type;
}

void EntityFactory::loadComponents(IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported)
{
	std::vector<CL_String> compTypes;

	//First add inherited components
	if(inheritSupported)
	{
		std::map<CL_String, std::vector<CL_String>>::iterator inheritIt = entity_components.find(inherit);
		if(inheritIt != entity_components.end())
		{
			std::vector<CL_String> inheritCompTypes = inheritIt->second;
			for(unsigned int i = 0; i < inheritCompTypes.size(); i++)
			{
				compTypes.push_back(inheritCompTypes[i]);
			}
		}
	}

	//Then add unique components
	XMLResource *cl_res = static_cast<XMLResource*>(res);
	std::vector<CL_DomNode> components = cl_res->getDoc().select_nodes("/Entity/Components/Component");
	for(unsigned int i = 0; i < components.size(); i++)
	{
		CL_DomElement compType = components[i].to_element();

		int alreadyExist = -1;
		for(unsigned int j = 0; j < compTypes.size(); j++)
		{
			if(compTypes[j] == compType.get_text())
			{
				alreadyExist = j;
				break;
			}
		}

		if(alreadyExist == -1)
			compTypes.push_back(compType.get_text());
	}
	entity_components[type] = compTypes;
}

void EntityFactory::loadProperties(IResource *res, const CL_String &type, const CL_String &inherit, bool inheritSupported)
{
	std::vector<std::pair<CL_String,CL_String>> propTypes;

	//First add inherited properties
	if(inheritSupported)
	{
		std::map<CL_String, std::vector<std::pair<CL_String,CL_String>>>::iterator inheritIt = entity_properties.find(inherit);
		if(inheritIt != entity_properties.end())
		{
			std::vector<std::pair<CL_String,CL_String>> inheritPropTypes = inheritIt->second;
			for(unsigned int i = 0; i < inheritPropTypes.size(); i++)
			{
				propTypes.push_back(inheritPropTypes[i]);
			}
		}
	}

	//Then add unique properties
	XMLResource *cl_res = static_cast<XMLResource*>(res);
	std::vector<CL_DomNode> properties = cl_res->getDoc().select_nodes("/Entity/Properties/Property");
	for(unsigned int i = 0; i < properties.size(); i++)
	{
		CL_DomElement propType = properties[i].to_element();
		CL_String propName = propType.get_child_string("Name");
		CL_String propValue = propType.get_child_string("Value");

		int alreadyExist = -1;
		for(unsigned int j = 0; j < propTypes.size(); j++)
		{
			if(propTypes[j].first == propName)
			{
				alreadyExist = j;
				break;
			}
		}

		//If property already exist, then the last property value will always win
		if(alreadyExist == -1)
			propTypes.push_back(std::pair<CL_String,CL_String>(propName, propValue));
		else
			propTypes[alreadyExist] = std::pair<CL_String,CL_String>(propName, propValue);
	}
	entity_properties[type] = propTypes;
}
