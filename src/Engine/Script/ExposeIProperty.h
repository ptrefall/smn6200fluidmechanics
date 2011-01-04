#pragma once

#include <LuaPlus/LuaPlus.h>
#include <Clanlib/src/API/core.h>

namespace Engine
{
class CoreMgr;
class IProperty;
class ExposeIEntity;
class ExposePropertyContainer;

class ExposeIProperty
{
public:
	ExposeIProperty(CoreMgr *coreMgr, ExposeIEntity *exposedEntity, ExposePropertyContainer *exposedPropContainer, IProperty *property);
	~ExposeIProperty();

	IProperty *getProp() const { return property; }
	LuaPlus::LuaObject getLProp() const { return lProperty; }

private:
	void init();

	LuaPlus::LuaObject Get(LuaPlus::LuaObject self);
	void Set(LuaPlus::LuaObject self, LuaPlus::LuaObject value);
	void AddListener(LuaPlus::LuaObject self, LuaPlus::LuaObject listener);

	void initPropertyListener();

	void OnPropertyChangedBool(const bool &oldValue, const bool &newValue);
	void OnPropertyChangedVec2f(const CL_Vec2f &oldValue, const CL_Vec2f &newValue);
	void OnPropertyChangedVec3f(const CL_Vec3f &oldValue, const CL_Vec3f &newValue);
	void OnPropertyChangedVec4f(const CL_Vec4f &oldValue, const CL_Vec4f &newValue);
	void OnPropertyChangedString(const CL_String &oldValue, const CL_String &newValue);
	void OnPropertyChangedDouble(const double &oldValue, const double &newValue);
	void OnPropertyChangedFloat(const float &oldValue, const float &newValue);
	void OnPropertyChangedInt(const int &oldValue, const int &newValue);
	void OnPropertyChangedUInt(const unsigned int &oldValue, const unsigned int &newValue);

	template<class T>
	void OnPropertyChanged(const T &oldValue, const T &newValue);
	
	CoreMgr *coreMgr;
	ExposePropertyContainer *exposedPropContainer;
	ExposeIEntity *exposedEntity;

	CL_String name;

	IProperty *property;
	LuaPlus::LuaObject lProperty;

	std::vector<CL_String> listeners;

	CL_Slot slotPropertyChanged;
};

}
