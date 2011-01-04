#include "precomp.h"
#include "ResFactory.h"
#include "XMLResource.h"

#include <Core/CoreMgr.h>

using namespace Engine;

std::map<CL_String, ResFactory::ResourceCreator>* ResFactory::creators;

ResFactory::ResFactory(CoreMgr *coreMgr)
{
	this->coreMgr = coreMgr;
}

ResFactory::~ResFactory()
{
	if(creators)
	{
		creators->clear();
		delete creators;
		creators = NULL;
	}
}

void ResFactory::registerResource(const CL_String &fileType, ResourceCreator functor)
{
	if(creators == 0)
		creators = new std::map<CL_String, ResourceCreator>();

	if(creators->find(fileType) == creators->end())
	{
		std::pair<CL_String, ResourceCreator> value(fileType, functor);
		creators->insert(value);
	}
}

IResource *ResFactory::create(const CL_String &fileName, const CL_String &fileType)
{
	if(creators == 0)
		throw CL_Exception("ResourceCreator map has not been instanciated!");

	//If we're trying to create a resource, but it's already been loaded into memory, don't do it twice!
	bool isXml = false;
	if(fileType == XMLResource::getType())
	{
		isXml = true;
		std::map<CL_String, IResource*>::iterator it = xmlResources.find(fileName);
		if(it != xmlResources.end())
		{
			return it->second;
		}
	}

	std::map<CL_String, ResourceCreator>::iterator creatorIt = creators->find(fileType);
	if(creatorIt == creators->end())
		throw CL_Exception(cl_format("%1 %2", "Unable to create resouce of type", fileType));
	
	ResourceCreator creator = creatorIt->second;
	
	IResource *resource = creator(coreMgr, fileName);
	if(resource && isXml)
	{
		xmlResources[fileName] = resource;
	}
	return resource;
}
