#pragma once

#include <ClanLib/src/API/core.h>

namespace Engine
{
class Component; 
class IProperty;
class IEntity;
namespace Events
{

class EventValue
{
public:
	enum Type
	{
		null,
		integer,
		uinteger,
		string,
		boolean,
		number,
		vec2,
		vec3,
		vec4,
		component,
		property,
		entity,
		gui_context,
		gui_document
	};

	EventValue();
	EventValue(int value);
	EventValue(unsigned int value);
	EventValue(float value);
	EventValue(const CL_Vec2f &value);
	EventValue(const CL_Vec3f &value);
	EventValue(const CL_Vec4f &value);
	EventValue(const CL_String &value);
	EventValue(const CL_StringRef &value);
	EventValue(const char *str);
	EventValue(const wchar_t *str);
	EventValue(Engine::Component *comp);
	EventValue(Engine::IProperty *prop);
	EventValue(Engine::IEntity *entity);
	EventValue(Rocket::Core::Context *context);
	EventValue(Rocket::Core::Element *document);
	explicit EventValue(bool value);
	EventValue(Type type);

	Type GetType() const;

	bool IsNull() const;
	bool IsUinteger() const;
	bool IsInteger() const;
	bool IsNumber() const;
	bool IsString() const;
	bool IsBoolean() const;
	bool IsVec2() const;
	bool IsVec3() const;
	bool IsVec4() const;
	bool IsComponent() const;
	bool IsProperty() const;
	bool IsEntity() const;
	bool IsGuiContext() const;
	bool IsGuiDocument() const;
	bool IsComplex() const;

	unsigned int ToUinteger() const;
	int ToInteger() const;
	float ToNumber() const;
	CL_String ToString() const;
	bool ToBoolean() const;
	CL_Vec2f ToVec2() const;
	CL_Vec3f ToVec3() const;
	CL_Vec4f ToVec4() const;
	Engine::Component *ToComponent() const;
	Engine::IProperty *ToProperty() const;
	Engine::IEntity *ToEntity() const;
	Rocket::Core::Context *ToGuiContext() const;
	Rocket::Core::Element *ToGuiDocument() const;

	inline operator unsigned int() const { return ToUinteger(); }
	inline operator int() const { return ToInteger(); }
	inline operator float() const { return ToNumber(); }
	inline operator CL_String() const { return ToString(); }
	inline operator bool() const { return ToBoolean(); }

private:
	Type type;
	union
	{
		int valueInt;
		unsigned int valueUint;
		float valueFloat;
		bool valueBool;
	};
	CL_String valueString;
	CL_Vec2f valueVec2;
	CL_Vec3f valueVec3;
	CL_Vec4f valueVec4;
	Engine::Component *valueComp;
	Engine::IProperty *valueProp;
	Engine::IEntity *valueEntity;
	Rocket::Core::Context *valueGuiContext;
	Rocket::Core::Element *valueGuiDocument;
};

}}
