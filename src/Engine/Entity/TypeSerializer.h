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

namespace Engine
{
class IProperty;

class TypeSerializer
{
// Supported types
public:
	enum TypeId
	{
		TYPE_UNSIGNED_INT = 1,
		TYPE_INT = 2,
		TYPE_FLOAT = 3,
		TYPE_DOUBLE = 4,
		TYPE_BOOL = 5,
        TYPE_STRING = 10,
		TYPE_VEC2 = 20,
		TYPE_VEC3 = 21,
		TYPE_VEC4 = 22,
		TYPE_MAT3 = 30,
		TYPE_MAT4 = 31
	};

// Factory
public:
	static IProperty *CreateProperty(TypeId typeId, const CL_String &name);

// Template functions
public:
	template<typename Type> 
	static CL_String ToString(const Type &value);

	template<typename Type> 
	static void FromString(const CL_String &inValue, Type &outValue);
	
	template<typename Type> 
	static TypeId GetTypeId(const Type &value);

// Template specializations
public:
	static CL_String ToString(const unsigned int &value);
	static void FromString(const CL_String &inValue, unsigned int &outValue);
	static TypeId GetTypeId(const unsigned int &value);

	static CL_String ToString(const int &value);
	static void FromString(const CL_String &inValue, int &outValue);
	static TypeId GetTypeId(const int &value);

	static CL_String ToString(const float &value);
	static void FromString(const CL_String &inValue, float &outValue);
	static TypeId GetTypeId(const float &value);

	static CL_String ToString(const double &value);
	static void FromString(const CL_String &inValue, double &outValue);
	static TypeId GetTypeId(const double &value);

	static CL_String ToString(const bool &value);
	static void FromString(const CL_String &inValue, bool &outValue);
	static TypeId GetTypeId(const bool &value);

	static CL_String ToString(const CL_String &value);
	static void FromString(const CL_String &inValue, CL_String &outValue);
	static TypeId GetTypeId(const CL_String &value);

	static CL_String ToString(const CL_Vec2f &value);
	static void FromString(const CL_String &inValue, CL_Vec2f &outValue);
	static TypeId GetTypeId(const CL_Vec2f &value);

	static CL_String ToString(const CL_Vec3f &value);
	static void FromString(const CL_String &inValue, CL_Vec3f &outValue);
	static TypeId GetTypeId(const CL_Vec3f &value);

	static CL_String ToString(const CL_Vec4f &value);
	static void FromString(const CL_String &inValue, CL_Vec4f &outValue);
	static TypeId GetTypeId(const CL_Vec4f &value);

	static CL_String ToString(const CL_Mat3f &value);
	static void FromString(const CL_String &inValue, CL_Mat3f &outValue);
	static TypeId GetTypeId(const CL_Mat3f &value);

	static CL_String ToString(const CL_Mat4f &value);
	static void FromString(const CL_String &inValue, CL_Mat4f &outValue);
	static TypeId GetTypeId(const CL_Mat4f &value);
};
}
