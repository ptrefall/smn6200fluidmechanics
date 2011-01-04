#include "precomp.h"
#include "ExposeIProperty.h"
#include "ExposeIEntity.h"
#include "ExposeEntityManager.h"
#include "ExposePropertyContainer.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>

#include <Entity/IProperty.h>
#include <Entity/TypeSerializer.h>
#include <Entity/IEntity.h>

#include <Event/Event.h>

using namespace Engine;
using namespace LuaPlus;

ExposeIProperty::ExposeIProperty(CoreMgr *coreMgr, ExposeIEntity *exposedEntity, ExposePropertyContainer *exposedPropContainer, IProperty *property)
{
	this->coreMgr = coreMgr;
	this->exposedEntity = exposedEntity;
	this->exposedPropContainer = exposedPropContainer;
	this->property = property;
	name = property->GetName();
	init();
}

ExposeIProperty::~ExposeIProperty()
{
	lProperty.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeIProperty::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	
	LuaObject &lProps = exposedPropContainer->getLProps();
	lProperty = lProps.CreateTable(property->GetName().c_str());
	lProperty.SetString("id", property->GetName().c_str());
	
	{
		LuaObject lMeta = lProperty.CreateTable("MetaTable");
		lMeta.SetObject("__index", lMeta);

		lMeta.RegisterDirect("Get", *this, &ExposeIProperty::Get);
		lMeta.RegisterDirect("Set", *this, &ExposeIProperty::Set);
		lMeta.RegisterDirect("AddListener", *this, &ExposeIProperty::AddListener);
		
		lProperty.SetLightUserData("__object", this);
		lProperty.SetMetaTable(lMeta);
	}

	{
		LuaObject lMeta;
		if(exposedEntity)
			lMeta = exposedEntity->getLMeta();

		lMeta.RegisterDirect(cl_format("Get%1", property->GetName()).c_str(), *this, &ExposeIProperty::Get);
		lMeta.RegisterDirect(cl_format("Set%1", property->GetName()).c_str(), *this, &ExposeIProperty::Set);
		lMeta.RegisterDirect(cl_format("Add%1Listener", property->GetName()).c_str(), *this, &ExposeIProperty::AddListener);
	}

	initPropertyListener();
}

void ExposeIProperty::initPropertyListener()
{
	if(property->GetTypeId() == TypeSerializer::TYPE_BOOL)
	{
		Property<bool> *prop = dynamic_cast<Property<bool> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedBool);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC2)
	{
		Property<CL_Vec2f> *prop = dynamic_cast<Property<CL_Vec2f> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedVec2f);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC3)
	{
		Property<CL_Vec3f> *prop = dynamic_cast<Property<CL_Vec3f> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedVec3f);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC4)
	{
		Property<CL_Vec4f> *prop = dynamic_cast<Property<CL_Vec4f> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedVec4f);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_STRING)
	{
		Property<CL_String> *prop = dynamic_cast<Property<CL_String> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedString);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_DOUBLE)
	{
		Property<double> *prop = dynamic_cast<Property<double> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedDouble);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_FLOAT)
	{
		Property<float> *prop = dynamic_cast<Property<float> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedFloat);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_INT)
	{
		Property<int> *prop = dynamic_cast<Property<int> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedInt);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_UNSIGNED_INT)
	{
		Property<unsigned int> *prop = dynamic_cast<Property<unsigned int> *>(property);
		if(prop)
			slotPropertyChanged = prop->ValueChanged().connect(this, &ExposeIProperty::OnPropertyChangedUInt);
	}
}

LuaObject ExposeIProperty::Get(LuaObject self)
{
	LuaObject lValue;

	if(exposedEntity)
	{
		std::map<CL_String, IProperty*>::iterator it = exposedEntity->getEntity()->GetProperties().find(property->GetName());
		if(it != exposedEntity->getEntity()->GetProperties().end())
		{
			if(property != it->second)
			{
				property = it->second;
			}
		}
	}

	if(property->GetTypeId() == TypeSerializer::TYPE_BOOL)
	{
		Property<bool> *prop = static_cast<Property<bool>*>(property);
		lValue.AssignBoolean(coreMgr->getScriptMgr()->GetGlobalState()->Get(), prop->Get());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC2)
	{
		Property<CL_Vec2f> *prop = static_cast<Property<CL_Vec2f>*>(property);
		lValue.AssignNewTable(coreMgr->getScriptMgr()->GetGlobalState()->Get());
		lValue.SetNumber("x", prop->Get().x);
		lValue.SetNumber("y", prop->Get().y);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC3)
	{
		Property<CL_Vec3f> *prop = static_cast<Property<CL_Vec3f>*>(property);
		lValue.AssignNewTable(coreMgr->getScriptMgr()->GetGlobalState()->Get());
		lValue.SetNumber("x", prop->Get().x);
		lValue.SetNumber("y", prop->Get().y);
		lValue.SetNumber("z", prop->Get().z);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC4)
	{
		Property<CL_Vec4f> *prop = static_cast<Property<CL_Vec4f>*>(property);
		lValue.AssignNewTable(coreMgr->getScriptMgr()->GetGlobalState()->Get());
		lValue.SetNumber("x", prop->Get().x);
		lValue.SetNumber("y", prop->Get().y);
		lValue.SetNumber("z", prop->Get().z);
		lValue.SetNumber("w", prop->Get().w);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_STRING)
	{
		lValue.AssignString(coreMgr->getScriptMgr()->GetGlobalState()->Get(), property->ToString().c_str());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_DOUBLE)
	{
		Property<double> *prop = static_cast<Property<double>*>(property);
		lValue.AssignNumber(coreMgr->getScriptMgr()->GetGlobalState()->Get(), (float)prop->Get());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_FLOAT)
	{
		Property<float> *prop = static_cast<Property<float>*>(property);
		lValue.AssignNumber(coreMgr->getScriptMgr()->GetGlobalState()->Get(), prop->Get());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_INT)
	{
		Property<int> *prop = static_cast<Property<int>*>(property);
		lValue.AssignInteger(coreMgr->getScriptMgr()->GetGlobalState()->Get(), prop->Get());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_UNSIGNED_INT)
	{
		Property<unsigned int> *prop = static_cast<Property<unsigned int>*>(property);
		lValue.AssignInteger(coreMgr->getScriptMgr()->GetGlobalState()->Get(), (int)prop->Get());
	}

	return lValue;
}

void ExposeIProperty::Set(LuaObject self, LuaObject value)
{
	if(property->GetTypeId() == TypeSerializer::TYPE_BOOL)
	{
		if(!value.IsBoolean())
			return;

		Property<bool> *prop = (Property<bool>*)property;
		prop->Set(value.GetBoolean());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC2)
	{
		if(!value.IsTable())
			return;

		Property<CL_Vec2f> *prop = (Property<CL_Vec2f>*)property;
		LuaObject xObj = value.GetByName("x");
		LuaObject yObj = value.GetByName("y");
		if(!xObj.IsNumber() || !yObj.IsNumber())
		{
			xObj = value.GetByIndex(0);
			yObj = value.GetByIndex(1);
			if(!xObj.IsNumber() || !yObj.IsNumber())
			{	
				return;
			}
		}

		CL_Vec2f val((float)xObj.ToNumber(), (float)yObj.ToNumber());
		prop->Set(val);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC3)
	{
		if(!value.IsTable())
			return;

		Property<CL_Vec3f> *prop = (Property<CL_Vec3f>*)property;
		LuaObject xObj = value.GetByName("x");
		LuaObject yObj = value.GetByName("y");
		LuaObject zObj = value.GetByName("z");
		if(!xObj.IsNumber() || !yObj.IsNumber() || !zObj.IsNumber())
		{
			xObj = value.GetByIndex(0);
			yObj = value.GetByIndex(1);
			zObj = value.GetByIndex(2);
			if(!xObj.IsNumber() || !yObj.IsNumber() || !zObj.IsNumber())
			{	
				return;
			}
		}

		CL_Vec3f val((float)xObj.ToNumber(), (float)yObj.ToNumber(), (float)zObj.ToNumber());
		prop->Set(val);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_VEC4)
	{
		if(!value.IsTable())
			return;

		Property<CL_Vec4f> *prop = (Property<CL_Vec4f>*)property;
		LuaObject xObj = value.GetByName("x");
		LuaObject yObj = value.GetByName("y");
		LuaObject zObj = value.GetByName("z");
		LuaObject wObj = value.GetByName("w");
		if(!xObj.IsNumber() || !yObj.IsNumber() || !zObj.IsNumber() || !wObj.IsNumber())
		{
			xObj = value.GetByIndex(0);
			yObj = value.GetByIndex(1);
			zObj = value.GetByIndex(2);
			wObj = value.GetByIndex(3);
			if(!xObj.IsNumber() || !yObj.IsNumber() || !zObj.IsNumber() || !wObj.IsNumber())
			{	
				return;
			}
		}

		CL_Vec4f val((float)xObj.ToNumber(), (float)yObj.ToNumber(), (float)zObj.ToNumber(), (float)wObj.ToNumber());
		prop->Set(val);
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_STRING)
	{
		if(!value.IsString())
			return;

		Property<CL_String> *prop = (Property<CL_String>*)property;
		prop->Set(CL_String(value.ToString()));
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_DOUBLE)
	{
		if(!value.IsNumber())
			return;

		Property<double> *prop = (Property<double>*)property;
		prop->Set(value.ToNumber());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_FLOAT)
	{
		if(!value.IsNumber())
		{
			CL_String err = cl_format("Property %1 is not a number, it's a %2 with value: %3", property->GetName(), value.TypeName(), value.ToString());
			throw CL_Exception(err);
		}

		Property<float> *prop = (Property<float>*)property;
		prop->Set((float)value.ToNumber());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_INT)
	{
		if(!value.IsInteger())
			return;

		Property<int> *prop = (Property<int>*)property;
		prop->Set(value.ToInteger());
	}
	else if(property->GetTypeId() == TypeSerializer::TYPE_UNSIGNED_INT)
	{
		if(!value.IsInteger())
			return;

		Property<unsigned int> *prop = (Property<unsigned int>*)property;
		prop->Set((unsigned int)value.ToInteger());
	}
}

void ExposeIProperty::AddListener(LuaPlus::LuaObject self, LuaPlus::LuaObject listener)
{
	if(!listener.IsString())
	{
		CL_String msg = cl_format("The listener has to be a string, but was a %1", listener.TypeName());
		throw CL_Exception(msg);
	}
	listeners.push_back(listener.ToString());
}

template<class T>
void ExposeIProperty::OnPropertyChanged(const T &oldValue, const T &newValue)
{
	for(unsigned int i = 0; i < listeners.size(); i++)
	{
		if(exposedEntity)
		{
			unsigned int id = 0;
			id = exposedEntity->getEntity()->getId();

			LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntity(id);
			if(lEntity.IsNil())
			{
				CL_String msg = cl_format("There's no entity in Entities table with id %1!", id);
				throw CL_Exception(msg);
			}

			CL_String script = cl_format("%1(Entities[%2], %3, %4)", listeners[i], id, oldValue, newValue);
			coreMgr->getScriptMgr()->doString(script);
		}
	}
}

void ExposeIProperty::OnPropertyChangedVec2f(const CL_Vec2f &oldValue, const CL_Vec2f &newValue)
{
	for(unsigned int i = 0; i < listeners.size(); i++)
	{
		if(exposedEntity)
		{
			unsigned int id = 0;
			id = exposedEntity->getEntity()->getId();

			LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntity(id);
			if(lEntity.IsNil())
			{
				CL_String msg = cl_format("There's no entity in Entities table with id %1!", id);
				throw CL_Exception(msg);
			}

			CL_String oldVal = cl_format("{x=%2, y=%3}", oldValue.x, oldValue.y);
			CL_String newVal = cl_format("{x=%6, y=%7}", newValue.x, newValue.y);
			CL_String script = cl_format("%1(Entities[%2], %3, %4)", listeners[i], id, oldVal, newVal);
			coreMgr->getScriptMgr()->doString(script);
		}
	}
}
void ExposeIProperty::OnPropertyChangedVec3f(const CL_Vec3f &oldValue, const CL_Vec3f &newValue)
{
	for(unsigned int i = 0; i < listeners.size(); i++)
	{
		if(exposedEntity)
		{
			unsigned int id = 0;
			id = exposedEntity->getEntity()->getId();

			LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntity(id);
			if(lEntity.IsNil())
			{
				CL_String msg = cl_format("There's no entity in Entities table with id %1!", id);
				throw CL_Exception(msg);
			}

			CL_String oldVal = cl_format("{x=%2, y=%3, z=%4}", oldValue.x, oldValue.y, oldValue.z);
			CL_String newVal = cl_format("{x=%6, y=%7, z=%8}", newValue.x, newValue.y, newValue.z);
			CL_String script = cl_format("%1(Entities[%2], %3, %4)", listeners[i], id, oldVal, newVal);
			coreMgr->getScriptMgr()->doString(script);
		}
	}
}
void ExposeIProperty::OnPropertyChangedVec4f(const CL_Vec4f &oldValue, const CL_Vec4f &newValue)
{
	for(unsigned int i = 0; i < listeners.size(); i++)
	{
		if(exposedEntity)
		{
			unsigned int id = 0;
			id = exposedEntity->getEntity()->getId();

			LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntity(id);
			if(lEntity.IsNil())
			{
				CL_String msg = cl_format("There's no entity in Entities table with id %1!", id);
				throw CL_Exception(msg);
			}

			CL_String oldVal = cl_format("{x=%2, y=%3, z=%4, w=%5}", oldValue.x, oldValue.y, oldValue.z, oldValue.w);
			CL_String newVal = cl_format("{x=%6, y=%7, z=%8, w=%9}", newValue.x, newValue.y, newValue.z, newValue.w);
			CL_String script = cl_format("%1(Entities[%2], %3, %4)", listeners[i], id, oldVal, newVal);
			coreMgr->getScriptMgr()->doString(script);
		}
	}
}
void ExposeIProperty::OnPropertyChangedBool(const bool &oldValue, const bool &newValue)
{
	for(unsigned int i = 0; i < listeners.size(); i++)
	{
		if(exposedEntity)
		{
			unsigned int id = 0;
			id = exposedEntity->getEntity()->getId();

			LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntity(id);
			if(lEntity.IsNil())
			{
				CL_String msg = cl_format("There's no entity in Entities table with id %1!", id);
				throw CL_Exception(msg);
			}

			CL_String script = cl_format("%1(Entities[%2], %3, %4)", listeners[i], id, CL_StringHelp::bool_to_text(oldValue), CL_StringHelp::bool_to_text(newValue));
			coreMgr->getScriptMgr()->doString(script);
		}
	}
}
void ExposeIProperty::OnPropertyChangedString(const CL_String &oldValue, const CL_String &newValue)
{
	for(unsigned int i = 0; i < listeners.size(); i++)
	{
		if(exposedEntity)
		{
			unsigned int id = 0;
			id = exposedEntity->getEntity()->getId();

			LuaObject lEntity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getLEntity(id);
			if(lEntity.IsNil())
			{
				CL_String msg = cl_format("There's no entity in Entities table with id %1!", id);
				throw CL_Exception(msg);
			}

			CL_String script = cl_format("%1(Entities[%2], '%3', '%4')", listeners[i], id, oldValue, newValue);
			coreMgr->getScriptMgr()->doString(script);
		}
	}
}
void ExposeIProperty::OnPropertyChangedDouble(const double &oldValue, const double &newValue)
{
	OnPropertyChanged<double>(oldValue, newValue);
}
void ExposeIProperty::OnPropertyChangedFloat(const float &oldValue, const float &newValue)
{
	OnPropertyChanged<float>(oldValue, newValue);
}
void ExposeIProperty::OnPropertyChangedInt(const int &oldValue, const int &newValue)
{
	OnPropertyChanged<int>(oldValue, newValue);
}
void ExposeIProperty::OnPropertyChangedUInt(const unsigned int &oldValue, const unsigned int &newValue)
{
	OnPropertyChanged<unsigned int>(oldValue, newValue);
}
