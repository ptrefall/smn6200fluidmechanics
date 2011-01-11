#pragma once

#include <Entity/IEntity.h>
#include <Core/ShaderObj.h>

namespace Engine
{
	class CoreMgr;
	class Flow3D : public IEntity
	{
	public:
		Flow3D(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory);
		virtual ~Flow3D();

		bool load(const CL_String &filename);
		void calcBufferData();

		static CL_String GetStaticSpecialType() { return "Flow3D"; }
		static IEntity* Create(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory) { return new Flow3D(id, type, name, coreMgr, factory); }
		virtual CL_String getSpecialType() const { return GetStaticSpecialType(); }

		virtual void Update(float dt);
		virtual void Render();
		
		//Animating over the flow3D frames
		void play();
		void stop();
		void change_frame(int i);

	private:
		void initIndices();
		void initVertices();
		void bindTexture();
		void unbindTexture();
		void bindUniforms();
		void updateMatrix(float dt);

		bool loadBinary(const CL_String &path);
		bool loadAscii(const CL_String &path);

		bool saveBinary();

		//Flow3D Specific variables
		struct Flow3dParamGroupFormat
		{
			CL_String type;
			bool found;
			std::vector<CL_String> params;
			Flow3dParamGroupFormat() : found(false) {}
		};
		struct Flow3dData
		{
			Flow3dParamGroupFormat format;
			std::vector<std::vector<float>> frame_data;
		};
		std::vector<CL_String> data_types;
		std::vector<Flow3dData> data;
		std::vector<Flow3dParamGroupFormat> flw_formats;
		//

		//Animation controllers
		int current_frame;
		bool play_frames;

		std::vector<unsigned int> frame_vao, frame_ibo, frame_vbo;
		std::vector<std::vector<unsigned int>> frame_indices;
		

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
