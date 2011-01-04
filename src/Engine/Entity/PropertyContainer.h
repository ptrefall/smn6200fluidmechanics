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

#include "IProperty.h"
#include "Property.h"

namespace Engine
{
class CoreMgr;
class Component;

class PropertyContainer
{
public:
	PropertyContainer(CoreMgr *coreMgr) { this->coreMgr = coreMgr; }
	virtual ~PropertyContainer() { RemoveAllProperties(); }

	bool HasProperty(const CL_String& propName);

	template<class T>Property<T> AddProperty(const CL_String& propName, const T &defaultValue);
	void AddProperty(IProperty *property);
	void PropertyAdded(IProperty *property);

	template<class T>Property<T> GetProperty(const CL_String& propName);
	IProperty *GetIProperty(const CL_String& propName);
	
	void RemoveProperty(const CL_String& propName);
	void RemoveAllProperties();
	
	std::map<CL_String, IProperty*> &GetProperties() { return properties; }

protected:
	std::map<CL_String, IProperty*> properties;
	CoreMgr *coreMgr;
};

inline bool PropertyContainer::HasProperty(const CL_String& propName)
{
	if(properties.empty())
		return false;

	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
		return true;
	else
		return false;
}

inline void PropertyContainer::AddProperty(IProperty *property)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(property->GetName());
	if(it == properties.end())
	{
		properties[property->GetName()] = property;
		PropertyAdded(property);
	}
}

template<class T>
inline Property<T> PropertyContainer::AddProperty(const CL_String& propName, const T &defaultValue)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		Property<T>* property = dynamic_cast< Property<T>* >(it->second);
		if(!property)
			throw CL_Exception("Property " + propName + " already exists, but with another type!");
		return *property;
	}

	Property<T> *property = new Property<T>(propName);
	property->Set(defaultValue);
	properties[property->GetName()] = property;
	PropertyAdded(property);

	//return *property;
	return GetProperty<T>(propName);
}

template<class T>
inline Property<T> PropertyContainer::GetProperty(const CL_String& propName)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		Property<T>* property = dynamic_cast< Property<T>* >(it->second);
		if(!property)
			throw CL_Exception("Tried to get property " + propName + ", but the type was wrong!");
		return *property;
	}
	else
		throw CL_Exception("Unable to get property " + propName);
}

inline IProperty *PropertyContainer::GetIProperty(const CL_String& propName)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		return it->second;
	}
	else
		throw CL_Exception("Unable to get property " + propName);
}

inline void PropertyContainer::RemoveProperty(const CL_String& propName)
{
	std::map<CL_String, IProperty*>::iterator it = properties.find(propName);
	if(it != properties.end())
	{
		IProperty* property = (*it).second;
		delete property;
		properties.erase(it);
	}
}

inline void PropertyContainer::RemoveAllProperties()
{
	std::map<CL_String, IProperty*>::iterator it;
	for(it = properties.begin(); it != properties.end(); ++it)
	{
		IProperty* property = (*it).second;
		delete property;
	}
	properties.clear();
}

}
