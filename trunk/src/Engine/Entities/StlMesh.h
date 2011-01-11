#pragma once

#include <Entity/IEntity.h>
#include <Core/ShaderObj.h>

namespace Engine
{
	struct StlHeader
	{
		unsigned char head[80];
		unsigned long int num_tris;
	};

	struct StlTri
	{
		float norm[3];
		float vert1[3];
		float vert2[3];
		float vert3[3];
		unsigned short attrib_count;
	};


	class CoreMgr;
	class StlMesh : public IEntity
	{
	public:
		StlMesh(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory);
		virtual ~StlMesh();

		bool load(const CL_String &filename);
		void calcBufferData();

		static CL_String GetStaticSpecialType() { return "StlMesh"; }
		static IEntity* Create(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory) { return new StlMesh(id, type, name, coreMgr, factory); }
		virtual CL_String getSpecialType() const { return GetStaticSpecialType(); }

		virtual void Update(float dt);
		virtual void Render();

	private:
		void initIndices();
		void initVertices();
		void bindTexture();
		void unbindTexture();
		void bindUniforms();
		void updateMatrix(float dt);

		StlHeader stl_header;
		StlTri *stl_tris;

		unsigned int vao, ibo, vbo;
		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		std::vector<float> normals;
		ShaderObj shader;
		bool solid;
		float size;
		bool shouldUpdate;
		bool loaded;

		unsigned int texture;

		CL_Quaternionf qPitch;
		CL_Quaternionf qHeading;

		CL_Vec3f direction;
		CL_Vec3f fowardDirection;
		CL_Vec3f leftDirection;
		CL_Vec3f upDirection;
		CL_Vec3f velocity;

		Property<CL_Vec3f> pos;
		Property<CL_Mat3f> rot;

		Property<bool> mirror;

		Property<float> pitch;
		Property<float> pitchRate;
		CL_Slot slotPitchChanged;
		void OnPitchChanged(const float &oldValue, const float &newValue);

		Property<float> yaw;
		Property<float> yawRate;
		CL_Slot slotYawChanged;
		void OnYawChanged(const float &oldValue, const float &newValue);

		Property<CL_String> mesh;
		CL_Slot slotMeshChanged;
		void OnMeshChanged(const CL_String &oldValue, const CL_String &newValue);
	};
}
