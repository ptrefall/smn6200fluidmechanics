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
#include "ComponentContainer.h"
#include "ComponentFactory.h"
#include "Component.h"
#include "IEntity.h"
#include "IProperty.h"
#include <Core/CoreMgr.h>
#include <Event/IEventManager.h>
#include <Event/Event.h>
#include <Event/EventValue.h>

using namespace Engine;

ComponentContainer::ComponentContainer(CoreMgr *coreMgr, ComponentFactory& factory) 
: coreMgr(coreMgr), factory(factory)
{ 
}

ComponentContainer::~ComponentContainer()
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
	{
		if((*it))
		{
			delete (*it);
			(*it) = NULL;
		}
	}

	components.clear();
}

Component* ComponentContainer::AddComponent(const CL_String& componentType, const CL_String& componentName)
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
	{
		Component *component = (*it);
		if(componentName.length() == 0)
		{
			if(component->GetName() == componentType)
				return component;
		}
		else
		{
			if(component->GetName() == componentName)
				return component;
		}
	}

	Component* component = factory.CreateComponent(static_cast<IEntity*>(this), componentType, componentName);
	if(component)
	{
		components.push_back(component);
		coreMgr->getEventMgr()->SendEvent(Events::Event("ComponentAdded", component, static_cast<IEntity*>(this)));
		return component;
	}
	else
		throw new CL_Exception("Unable to create component " + componentType);
}

bool ComponentContainer::HasComponent(const CL_String& componentName)
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
	{
		Component *component = (*it);
		if(component->GetName() == componentName)
			return true;
	}
	return false;
}

Component* ComponentContainer::GetComponent(const CL_String& componentName)
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
	{
		Component *component = (*it);
		if(component->GetName() == componentName)
		{
			return component;
		}
	}
	throw CL_Exception("Unable to get component " + componentName);
}

void ComponentContainer::UpdateComponents(double deltaTime)
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
		(*it)->Update(deltaTime);
}

void ComponentContainer::ExecuteCommandOnComponents(const CL_String &command)
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
		(*it)->ExecuteCommand(command);
}

void ComponentContainer::ExecuteEventOnComponents(const Events::Event &event)
{
	std::vector<Component*>::iterator it;
	for(it = components.begin(); it != components.end(); ++it)
		(*it)->ExecuteEvent(event);
}
