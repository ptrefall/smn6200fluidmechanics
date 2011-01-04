/*Component-based Entity Engine
Copyright (c) 2009 Pål Trefall and Kenneth Gangstø

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Component-based Entity Engine may link to may have additional
requirements or restrictions.
*/

#include "precomp.h"
#include "ComponentFactory.h"
#include "Component.h"

using namespace Engine;

std::map<CL_String, ComponentFactory::ComponentCreator>* ComponentFactory::creators;

ComponentFactory::ComponentFactory(CoreMgr *coreMgr)
{
	this->coreMgr = coreMgr;
}

ComponentFactory::~ComponentFactory()
{
	if(creators)
	{
		creators->clear();
		delete creators;
	}
}

void ComponentFactory::RegisterComponent(const CL_String& type, ComponentCreator functor)
{
	if(creators == 0)
		creators = new std::map<CL_String, ComponentCreator>();

	if(creators->find(type) == creators->end())
	{
		std::pair<CL_String, ComponentCreator> value(type, functor);
		creators->insert(value);
	}
}

Component* ComponentFactory::CreateComponent(IEntity *entity, const CL_String& compType, const CL_String& compName)
{
	if(creators == 0)
		return 0;

	std::map<CL_String, ComponentCreator>::iterator creatorIt = creators->find(compType);
	if(creatorIt == creators->end())
		throw CL_Exception("Unable to create component " + compType);
	
	ComponentCreator creator = creatorIt->second;

	if(compName.empty())
		return creator(coreMgr, entity, compType);
	else
		return creator(coreMgr, entity, compName);
}
