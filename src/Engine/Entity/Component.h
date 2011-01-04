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

#pragma once

#include "Property.h"
#include "ComponentContainer.h"
#include "PropertyContainer.h"
#include "IEntity.h"
#include <Event/CommandHandler.h>
#include <Event/Event.h>

namespace Engine
{
class Component
{
public:
	virtual ~Component() {}

	virtual CL_String GetName() const { return name; }

    virtual void Update(double deltaTime) {};
	virtual void ExecuteCommand(const CL_String &command) {};
	virtual void ExecuteEvent(const Events::Event &event) {};

	template<class T>Property<T> AddProperty(const CL_String &propName, const T &defaultValue);
	template<class T>Property<T> GetProperty(const CL_String &propName);
	void RemoveProperty(const CL_String &propName);

protected:
	Component(IEntity *entity, const CL_String &name) : entity(entity), name(name) {};

	IEntity *entity;
    CL_String name;
};

template<class T>
inline Property<T> Component::AddProperty(const CL_String &propName, const T &defaultValue)
{
	CL_String compPropName = name + ":" + propName;
	return entity->AddProperty<T>(compPropName, defaultValue);
}

template<class T>
inline Property<T> Component::GetProperty(const CL_String &propName)
{
	CL_String compPropName = name + ":" + propName;
	return entity->GetProperty<T>(compPropName);
}

inline void Component::RemoveProperty(const CL_String &propName)
{
	CL_String compPropName = name + ":" + propName;
	entity->RemoveProperty(compPropName);
}

}
