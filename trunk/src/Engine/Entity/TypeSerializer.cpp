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
#include "Property.h"
#include "TypeSerializer.h"

using namespace Engine;

/////////////////////////////////////////////////////////////////////////////
// Factory

IProperty *TypeSerializer::CreateProperty(TypeId typeId, const CL_String &name)
{
	switch(typeId)
	{
	case TypeSerializer::TYPE_UNSIGNED_INT:
		return new Property<unsigned int>(name);
	case TypeSerializer::TYPE_INT:
		return new Property<int>(name);
	case TypeSerializer::TYPE_FLOAT:
		return new Property<float>(name);
	case TypeSerializer::TYPE_DOUBLE:
		return new Property<double>(name);
	case TypeSerializer::TYPE_BOOL:
		return new Property<bool>(name);
	case TypeSerializer::TYPE_STRING:
		return new Property<CL_String>(name);
	case TypeSerializer::TYPE_VEC2:
		return new Property<CL_Vec2f>(name);
	case TypeSerializer::TYPE_VEC3:
		return new Property<CL_Vec3f>(name);
	case TypeSerializer::TYPE_VEC4:
		return new Property<CL_Vec4f>(name);
	case TypeSerializer::TYPE_MAT3:
		return new Property<CL_Mat3f>(name);
	case TypeSerializer::TYPE_MAT4:
		return new Property<CL_Mat4f>(name);
	default:
		throw CL_Exception(cl_format("TypeSerializer::CreateProperty - Unknown entity type %1", typeId));
	}
}

/////////////////////////////////////////////////////////////////////////////
// unsigned int

CL_String TypeSerializer::ToString(const unsigned int &value)
{
	return CL_StringHelp::uint_to_text(value);
}

void TypeSerializer::FromString(const CL_String &value, unsigned int &outValue)
{
	outValue = CL_StringHelp::text_to_uint(value);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const unsigned int &value)
{
	return TYPE_UNSIGNED_INT;
}

/////////////////////////////////////////////////////////////////////////////
// int

CL_String TypeSerializer::ToString(const int &value)
{
	return CL_StringHelp::int_to_text(value);
}

void TypeSerializer::FromString(const CL_String &value, int &outValue)
{
	outValue = CL_StringHelp::text_to_int(value);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const int &value)
{
	return TYPE_INT;
}

/////////////////////////////////////////////////////////////////////////////
// float

CL_String TypeSerializer::ToString(const float &value)
{
	return CL_StringHelp::float_to_text(value);
}

void TypeSerializer::FromString(const CL_String &value, float &outValue)
{
	outValue = CL_StringHelp::text_to_float(value);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const float &value)
{
	return TYPE_FLOAT;
}

/////////////////////////////////////////////////////////////////////////////
// double

CL_String TypeSerializer::ToString(const double &value)
{
	return CL_StringHelp::double_to_text(value);
}

void TypeSerializer::FromString(const CL_String &value, double &outValue)
{
	outValue = CL_StringHelp::text_to_double(value);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const double &value)
{
	return TYPE_DOUBLE; 
}

/////////////////////////////////////////////////////////////////////////////
// bool

CL_String TypeSerializer::ToString(const bool &value)
{
	return CL_StringHelp::bool_to_text(value);
}

void TypeSerializer::FromString(const CL_String &value, bool &outValue)
{
	outValue = CL_StringHelp::text_to_bool(value);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const bool &value)
{
	return TYPE_BOOL;
}

/////////////////////////////////////////////////////////////////////////////
// CL_String

CL_String TypeSerializer::ToString(const CL_String &value)
{
	return value;
}

void TypeSerializer::FromString(const CL_String &value, CL_String &outValue)
{
	outValue = value;
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const CL_String &value)
{
	return TYPE_STRING;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Vec2f

CL_String TypeSerializer::ToString(const CL_Vec2f &value)
{
	return CL_StringHelp::float_to_text(value.x) + "|" + CL_StringHelp::float_to_text(value.y);
}

void TypeSerializer::FromString(const CL_String &value, CL_Vec2f &outValue)
{
	std::vector<CL_String> values = CL_StringHelp::split_text(value, "|");
	if(values.size() != 2)
	{
		values = CL_StringHelp::split_text(value, ",");
		if(values.size() != 2)
			throw CL_Exception("Vec2f FromString failed");
	}

	float x = CL_StringHelp::text_to_float(values[0]);
	float y = CL_StringHelp::text_to_float(values[1]);

	outValue = CL_Vec2f(x, y);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const CL_Vec2f &value)
{
	return TYPE_VEC2;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Vec3f

CL_String TypeSerializer::ToString(const CL_Vec3f &value)
{
	return CL_StringHelp::float_to_text(value.x) + "|" + CL_StringHelp::float_to_text(value.y) + "|" + CL_StringHelp::float_to_text(value.z);
}

void TypeSerializer::FromString(const CL_String &value, CL_Vec3f &outValue)
{
	std::vector<CL_String> values = CL_StringHelp::split_text(value, "|");
	if(values.size() != 3)
	{
		values = CL_StringHelp::split_text(value, ",");
		if(values.size() != 3)
			throw CL_Exception("Vec3f FromString failed");
	}

	float x = CL_StringHelp::text_to_float(values[0]);
	float y = CL_StringHelp::text_to_float(values[1]);
	float z = CL_StringHelp::text_to_float(values[2]);

	outValue = CL_Vec3f(x, y, z);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const CL_Vec3f &value)
{
	return TYPE_VEC3;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Vec4f

CL_String TypeSerializer::ToString(const CL_Vec4f &value)
{
	return CL_StringHelp::float_to_text(value.x) + "|" + CL_StringHelp::float_to_text(value.y) + "|" + CL_StringHelp::float_to_text(value.z) + "|" + CL_StringHelp::float_to_text(value.w);
}

void TypeSerializer::FromString(const CL_String &value, CL_Vec4f &outValue)
{
	std::vector<CL_String> values = CL_StringHelp::split_text(value, "|");
	if(values.size() != 4)
	{
		values = CL_StringHelp::split_text(value, ",");
		if(values.size() != 4)
			throw CL_Exception("Vec4f FromString failed");
	}

	float x = CL_StringHelp::text_to_float(values[0]);
	float y = CL_StringHelp::text_to_float(values[1]);
	float z = CL_StringHelp::text_to_float(values[2]);
	float w = CL_StringHelp::text_to_float(values[3]);

	outValue = CL_Vec4f(x, y, z, w);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const CL_Vec4f &value)
{
	return TYPE_VEC4;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mat3f

CL_String TypeSerializer::ToString(const CL_Mat3f &value)
{
	return CL_StringHelp::float_to_text(value[0]) + "|" + CL_StringHelp::float_to_text(value[1]) + "|" + CL_StringHelp::float_to_text(value[2]) 
		+ "|" + CL_StringHelp::float_to_text(value[3]) + "|" + CL_StringHelp::float_to_text(value[4]) + "|" + CL_StringHelp::float_to_text(value[5])
		+ "|" + CL_StringHelp::float_to_text(value[6]) + "|" + CL_StringHelp::float_to_text(value[7]) + "|" + CL_StringHelp::float_to_text(value[8]);
}

void TypeSerializer::FromString(const CL_String &value, CL_Mat3f &outValue)
{
	std::vector<CL_String> values = CL_StringHelp::split_text(value, "|");
	if(values.size() != 9)
		throw CL_Exception("Mat3f FromString failed");

	float a = CL_StringHelp::text_to_float(values[0]);
	float b = CL_StringHelp::text_to_float(values[1]);
	float c = CL_StringHelp::text_to_float(values[2]);

	float d = CL_StringHelp::text_to_float(values[3]);
	float e = CL_StringHelp::text_to_float(values[4]);
	float f = CL_StringHelp::text_to_float(values[5]);

	float g = CL_StringHelp::text_to_float(values[6]);
	float h = CL_StringHelp::text_to_float(values[7]);
	float i = CL_StringHelp::text_to_float(values[8]);

	outValue = CL_Mat3f(a,b,c, d,e,f, g,h,i);
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const CL_Mat3f &value)
{
	return TYPE_MAT3;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Mat4f

CL_String TypeSerializer::ToString(const CL_Mat4f &value)
{
	return CL_StringHelp::float_to_text(value[0]) + "|" + CL_StringHelp::float_to_text(value[1]) + "|" + CL_StringHelp::float_to_text(value[2]) + "|" + CL_StringHelp::float_to_text(value[3]) 
		+ "|" + CL_StringHelp::float_to_text(value[4]) + "|" + CL_StringHelp::float_to_text(value[5]) + "|" + CL_StringHelp::float_to_text(value[6]) + "|" + CL_StringHelp::float_to_text(value[7]) 
		+ "|" + CL_StringHelp::float_to_text(value[8]) + "|" + CL_StringHelp::float_to_text(value[9]) + "|" + CL_StringHelp::float_to_text(value[10]) + "|" + CL_StringHelp::float_to_text(value[11])
		+ "|" + CL_StringHelp::float_to_text(value[12]) + "|" + CL_StringHelp::float_to_text(value[13]) + "|" + CL_StringHelp::float_to_text(value[14]) + "|" + CL_StringHelp::float_to_text(value[15]);
}

void TypeSerializer::FromString(const CL_String &value, CL_Mat4f &outValue)
{
	std::vector<CL_String> values = CL_StringHelp::split_text(value, "|");
	if(values.size() != 16)
		throw CL_Exception("Mat4f FromString failed");

	float *init_values = new float[16];
	init_values[0] = CL_StringHelp::text_to_float(values[0]);
	init_values[1] = CL_StringHelp::text_to_float(values[1]);
	init_values[2] = CL_StringHelp::text_to_float(values[2]);
	init_values[3] = CL_StringHelp::text_to_float(values[3]);

	init_values[4] = CL_StringHelp::text_to_float(values[4]);
	init_values[5] = CL_StringHelp::text_to_float(values[5]);
	init_values[6] = CL_StringHelp::text_to_float(values[6]);
	init_values[7] = CL_StringHelp::text_to_float(values[7]);

	init_values[8] = CL_StringHelp::text_to_float(values[8]);
	init_values[9] = CL_StringHelp::text_to_float(values[9]);
	init_values[10] = CL_StringHelp::text_to_float(values[10]);
	init_values[11] = CL_StringHelp::text_to_float(values[11]);

	init_values[12] = CL_StringHelp::text_to_float(values[12]);
	init_values[13] = CL_StringHelp::text_to_float(values[13]);
	init_values[14] = CL_StringHelp::text_to_float(values[14]);
	init_values[15] = CL_StringHelp::text_to_float(values[15]);

	outValue = CL_Mat4f(init_values);

	delete init_values;
}

TypeSerializer::TypeId TypeSerializer::GetTypeId(const CL_Mat4f &value)
{
	return TYPE_MAT4;
}
