#pragma once

#include <Entity/IEntity.h>
#include <Core/ShaderObj.h>

namespace Engine
{
	class CoreMgr;
	class Volume : public IEntity
	{
	public:
		Volume(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory);
		virtual ~Volume();

		static CL_String GetStaticSpecialType() { return "Volume"; }
		static IEntity* Create(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory) { return new Volume(id, type, name, coreMgr, factory); }
		virtual CL_String getSpecialType() const { return GetStaticSpecialType(); }

		virtual void Render();

	private:
		void initIndices();
		void initVertices();
		void bindUniforms();

		unsigned int vao, ibo, vbo;
		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> texCoords;
		ShaderObj shader;
		bool solid;
		float size;

		Property<CL_Vec3f> pos;
		Property<CL_Mat3f> rot;
	};
}
