#include "precomp.h"
#include "ExposePropertyContainer.h"
#include "ExposeIProperty.h"
#include "ExposeIEntity.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>

#include <Event/IEventManager.h>
#include <Entity/IEntity.h>

#include <Event/Event.h>

using namespace Engine;
using namespace LuaPlus;

ExposePropertyContainer::ExposePropertyContainer(CoreMgr *coreMgr, ExposeIEntity *exposedEntity)
: engineEvents(coreMgr->getEventMgr())
{
	this->coreMgr = coreMgr;
	this->exposedEntity = exposedEntity;
	init();
}

ExposePropertyContainer::~ExposePropertyContainer()
{
	for(unsigned int i = 0; i < exposedProperties.size(); i++)
	{
		ExposeIProperty *exposedProp = exposedProperties[i];
		delete exposedProp;
		exposedProp = NULL;
	}
	exposedProperties.clear();
	lProperties.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposePropertyContainer::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	
	if(exposedEntity)
	{
		lProperties = exposedEntity->getLEntity().CreateTable("Properties");

		IEntity *entity = exposedEntity->getEntity();
		std::map<CL_String, IProperty*> &properties = entity->GetProperties();
		std::map<CL_String, IProperty*>::iterator propIt = properties.begin();
		for(; propIt != properties.end(); ++propIt)
		{
			ExposeIProperty *exposedProp = new ExposeIProperty(coreMgr, exposedEntity, this, propIt->second);
			exposedProperties.push_back(exposedProp);
		}

		LuaObject lMeta = exposedEntity->getLMeta();
		lMeta.RegisterDirect("AddProperty", *this, &ExposePropertyContainer::AddProperty);
		lMeta.RegisterDirect("HasProperty", *this, &ExposePropertyContainer::HasProperty);
	}

	engineEvents.Connect("PropertyAdded", this, &ExposePropertyContainer::OnPropertyAdded);
}

void ExposePropertyContainer::AddProperty(LuaObject self, LuaObject lName, LuaObject defValue)
{
	if(!self.IsTable())
	{
		CL_String msg = cl_format("Failed to add property, because the type of self was %1 when expecting Table", self.TypeName());
		throw CL_Exception(msg);
	}

	if(!lName.IsString())
	{
		CL_String msg = cl_format("Failed to add property, because the type of name was %1 when expecting String", lName.TypeName());
		throw CL_Exception(msg);
	}

	CL_String name = lName.ToString();
	
	IEntity *entity = NULL;
	if(exposedEntity)
		entity = exposedEntity->getEntity();

	if(defValue.IsBoolean())
	{
		bool val = defValue.GetBoolean();
		if(entity)
			entity->AddProperty<bool>(name, val);
	}
	else if(defValue.IsNumber())
	{
		float val = (float)defValue.ToNumber();
		if(entity)
			entity->AddProperty<float>(name, val);
	}
	else if(defValue.IsInteger())
	{
		int val = defValue.ToInteger();
		if(entity)
			entity->AddProperty<int>(name, val);
	}
	else if(defValue.IsString())
	{
		CL_String val = defValue.ToString();
		if(entity)
			entity->AddProperty<CL_String>(name, val);
	}
	else if(defValue.IsTable())
	{
		bool hasX = false;
		bool hasY = false;
		bool hasZ = false;
		bool hasW = false;

		LuaObject xObj = defValue.GetByName("x");
		if(xObj.IsNumber())
			hasX = true;
		
		LuaObject yObj = defValue.GetByName("y");
		if(yObj.IsNumber())
			hasY = true;
		
		LuaObject zObj = defValue.GetByName("z");
		if(zObj.IsNumber())
			hasZ = true;
		
		LuaObject wObj = defValue.GetByName("w");
		if(wObj.IsNumber())
			hasW = true;

		if(hasX && hasY && hasZ && hasW)
		{
			CL_Vec4f val = CL_Vec4f((float)xObj.ToNumber(),(float)yObj.ToNumber(),(float)zObj.ToNumber(),(float)wObj.ToNumber());
			if(entity)
				entity->AddProperty<CL_Vec4f>(name, val);
		}
		else if(hasX && hasY && hasZ)
		{
			CL_Vec3f val = CL_Vec3f((float)xObj.ToNumber(),(float)yObj.ToNumber(),(float)zObj.ToNumber());
			if(entity)
				entity->AddProperty<CL_Vec3f>(name, val);
		}
		else if(hasX && hasY)
		{
			CL_Vec2f val = CL_Vec2f((float)xObj.ToNumber(),(float)yObj.ToNumber());
			if(entity)
				entity->AddProperty<CL_Vec2f>(name, val);
		}
	}
	return;
}

LuaPlus::LuaObject ExposePropertyContainer::HasProperty(LuaPlus::LuaObject self, LuaPlus::LuaObject name)
{
	if(!self.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", self.TypeName());
		throw CL_Exception(msg);
	}

	if(!name.IsString())
	{
		CL_String msg = cl_format("Name was not a string (it's a %1)", name.TypeName());
		throw CL_Exception(msg);
	}

	bool retVal = false;
	if(exposedEntity)
		retVal = exposedEntity->getEntity()->HasProperty(name.ToString());

	LuaObject lRetVal;
	lRetVal.AssignBoolean(coreMgr->getScriptMgr()->GetGlobalState()->Get(), retVal);
	return lRetVal;
}

void ExposePropertyContainer::add(const CL_String &name, IProperty *prop, ExposeIProperty *exposedProp)
{
	if(prop == NULL)
	{
		CL_String msg = cl_format("Failed to add property %1, because no property was returned", name);
		throw CL_Exception(msg);
	}
	
	if(exposedEntity)
	{
		exposedProp = new ExposeIProperty(coreMgr, exposedEntity, this, prop);
		exposedProperties.push_back(exposedProp);
	}
}

void ExposePropertyContainer::OnPropertyAdded(const Events::Event &event)
{
	if(event.getArgument(1).IsEntity() && exposedEntity)
	{
		IEntity *entity = event.getArgument(1).ToEntity();
		if(entity->getId() != exposedEntity->getEntity()->getId())
			return;
	}

	IProperty *prop = event.getArgument(0).ToProperty();
	ExposeIProperty *exposedProp = NULL;
	add(prop->GetName(), prop, exposedProp);
}
