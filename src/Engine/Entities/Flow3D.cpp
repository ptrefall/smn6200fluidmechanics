#include "precomp.h"
#include "Flow3D.h"
#include <Event/Event.h>
#include <Event/EventValue.h>
#include <Core/CoreMgr.h>
#include <Core/Cam.h>
#include <Resource/ResMgr.h>
#include <Resource/TexResource.h>
#include <Resource/IResource.h>
#include <Resource/XMLResource.h>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace Engine;

Flow3D::Flow3D(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory)
: IEntity(id, type, name, coreMgr, factory), solid(true), size(4.0f), shouldUpdate(false), loaded(false), current_frame(0), play_frames(false)
{
	pos = this->AddProperty<CL_Vec3f>("Position", CL_Vec3f(0,0,0));
	rot = this->AddProperty<CL_Mat3f>("RotationMatrix", CL_Mat3f::identity());
	
	pitch = this->AddProperty<float>("Pitch", 0.0f);
	pitchRate = this->AddProperty<float>("PitchRate", 1.0f);
	
	yaw = this->AddProperty<float>("Yaw", 0.0f);
	yawRate = this->AddProperty<float>("YawRate", 1.0f);

	mesh = this->AddProperty<CL_String>("Mesh", CL_String());
	
	slotPitchChanged = pitch.ValueChanged().connect(this, &Flow3D::OnPitchChanged);
	slotYawChanged = yaw.ValueChanged().connect(this, &Flow3D::OnYawChanged);
	slotMeshChanged = mesh.ValueChanged().connect(this, &Flow3D::OnMeshChanged);

	upDirection = CL_Vec3f(0.0f, 1.0f, 0.0f);

	TexResource::beginTextureCube(texture);
	TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "X+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "X-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Y+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Y-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Z+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Z-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	TexResource::endTextureCube(texture);

	IResource *i_cfg = coreMgr->getResMgr()->create("flow3d_params.xml", "XML");
	XMLResource *cfg = static_cast<XMLResource*>(i_cfg);
	std::vector<CL_DomNode> paramGroups = cfg->getDoc().select_nodes("/Flow3D_Params/ParamGroups/ParamGroup");
	for(unsigned int i = 0; i < paramGroups.size(); i++)
	{
		CL_DomElement paramGroup = paramGroups[i].to_element();
		
		Flow3dParamGroupFormat format;
		format.type = paramGroup.get_child_string("Type");
		
		std::vector<CL_DomNode> params = paramGroup.select_nodes("Params/Param");
		for(unsigned int j = 0; j < params.size(); j++)
		{
			CL_DomElement param = params[j].to_element();
			CL_String paramName = param.get_text();
			format.params.push_back(paramName);
		}
		flw_formats.push_back(format);
	}
}

Flow3D::~Flow3D()
{
	unsigned int num_frames = frame_ibo.size();
	glDeleteBuffers(num_frames, &frame_ibo[0]);
	glDeleteBuffers(num_frames, &frame_vbo[0]);
	glDeleteVertexArrays(num_frames, &frame_vao[0]);
}

bool Flow3D::load(const CL_String &filename)
{
	if(loaded)
		return false;

	CL_String binary_file = cl_format("%1.bin", filename);
	std::vector<CL_String> files = coreMgr->getResMgr()->getFilesInDir("/Sim/");
	for(unsigned int i = 0; i < files.size(); i++)
	{
		if(files[i] == binary_file)
		{
			loadBinary(cl_format("%1Sim/%2.bin", coreMgr->getResMgr()->getRootPath(), filename));
			calcBufferData();
			return true;
		}
	}

	CL_String ascii_file = cl_format("%1.txt", filename);
	for(unsigned int i = 0; i < files.size(); i++)
	{
		if(files[i] == ascii_file)
		{
			loadAscii(cl_format("%1Sim/%2.txt", coreMgr->getResMgr()->getRootPath(), filename));
			calcBufferData();
			return true;
		}
	}

	return false;
}

void Flow3D::Update(float dt)
{
	if(!loaded)
		return;

	if(shouldUpdate)
	{
		updateMatrix(dt);
		shouldUpdate = false;
	}

	if(play_frames)
	{
		change_frame(1);
	}
}

void Flow3D::Render()
{
	if(!loaded)
		return;

	glEnable(GL_POINT_SMOOTH);
	glPointSize(10.0f);

	shader.enableShader();
	{
		bindTexture();
		bindUniforms();
		glBindVertexArray(frame_vao[current_frame]);
		//glDrawElements(GL_POINTS, 10, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glDrawArrays(GL_POINTS, 0, data[0].frame_data[current_frame].size());
		glBindVertexArray(0);
		unbindTexture();
	} shader.disableShader();

	glDisable(GL_POINT_SMOOTH);
}

void Flow3D::bindUniforms()
{
	const CL_Mat4f &viewMat = coreMgr->getCam()->getViewMatrix();
	const CL_Mat4f &projMat = coreMgr->getCam()->getProjectionMatrix();
	
	CL_Mat4f translateMat = CL_Mat4f::identity();
	translateMat[12] = pos.Get().x;
	translateMat[13] = pos.Get().y;
	translateMat[14] = pos.Get().z;
	
	CL_Mat4f modelMat = CL_Mat4f::identity();
	modelMat = modelMat * rot.Get();
	modelMat = modelMat * translateMat;
	
	CL_Mat4f mvMat = modelMat * viewMat;

	CL_Mat4f normMat = CL_Mat4f::identity();
	normMat[0] = mvMat[0];
	normMat[1] = mvMat[1];
	normMat[2] = mvMat[2];
	normMat[3] = 0.0f;

	normMat[4] = mvMat[4];
	normMat[5] = mvMat[5];
	normMat[6] = mvMat[6];
	normMat[7] = 0.0f;

	normMat[8] = mvMat[8];
	normMat[9] = mvMat[9];
	normMat[10] = mvMat[10];
	normMat[11] = 0.0f;

	normMat[12] = 0.0f;
	normMat[13] = 0.0f;
	normMat[14] = 0.0f;
	normMat[15] = 0.0f;

	//normMat.inverse();
	normMat.transpose();

	int loc = glGetUniformLocation(shader.getShaderProg(), "projMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &projMat[0]);
	
	loc = glGetUniformLocation(shader.getShaderProg(), "mvMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mvMat[0]);

	loc = glGetUniformLocation(shader.getShaderProg(), "normMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &normMat[0]);

	loc = glGetUniformLocation(shader.getShaderProg(), "fCube");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniform1i(loc, 0);

	loc = glGetUniformLocation(shader.getShaderProg(), "fAlpha");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniform1f(loc, alpha.Get());

	loc = glGetUniformLocation(shader.getShaderProg(), "vScale");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniform1f(loc, scale.Get());
}

void Flow3D::bindTexture()
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void Flow3D::unbindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void Flow3D::updateMatrix(float dt)
{
	CL_Mat4f mat;
	CL_Quaternionf q;
	qPitch = CL_Quaternionf::axis_angle(CL_Angle(pitch.Get(), cl_radians), CL_Vec3f(1.0f, 0.0f, 0.0f));
	qHeading = CL_Quaternionf::axis_angle(CL_Angle(yaw.Get(), cl_radians), CL_Vec3f(0.0f, 1.0f, 0.0f));
	
	q = qPitch * qHeading;
	rot = q.to_matrix();

	mat = qPitch.to_matrix();
	fowardDirection.y = mat[9];

	q = qHeading * qPitch;
	mat = q.to_matrix();
	fowardDirection.x = mat[8];
	fowardDirection.z = mat[10];
	leftDirection = CL_Vec3f::cross(fowardDirection, upDirection);
}

void Flow3D::OnPitchChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}

void Flow3D::OnYawChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}

void Flow3D::OnMeshChanged(const CL_String &oldValue, const CL_String &newValue)
{
	if(!load(newValue))
		throw CL_Exception(cl_format("Failed to load STL mesh %1", newValue));

	loaded = true;
}

bool Flow3D::loadBinary(const CL_String &path)
{
	//Open file
	std::ifstream fin(path.c_str(), std::ios::binary);
	if(fin.bad())
	{
		fin.close();
		CL_Console::write_line(cl_format("Could not open flow3d bin file %1", path));
		return false;
	}

	//Header

	//Close file
	fin.close();
	return true;
}

bool Flow3D::loadAscii(const CL_String &path)
{
	//Open file
	std::ifstream fin(path.c_str());
	if(fin.bad())
	{
		fin.close();
		CL_Console::write_line(cl_format("Could not open flow3d ascii file %1", path));
		return false;
	}

	//Header ignore
	for(int i = 0; i < 5; i++)
		fin.ignore(256, '\n');

	std::string lineread;
	std::getline(fin, lineread);
	std::getline(fin, lineread);
	std::getline(fin, lineread);
	std::getline(fin, lineread); //scalar-field header

	std::istringstream iss;

	bool frameHeaderExtracted = false;

	//Iterate over frames
	while(std::getline(fin, lineread)) //frame-data header
	{
		std::getline(fin, lineread); //data-type header

		if(frameHeaderExtracted == false)
		{
			CL_String line_read(lineread.c_str());
			data_types = CL_StringHelp::split_text(line_read, " ");
			
			//Go over the data types header and find which
			//data formats we can expect from this file,
			//sorted by data type
			for(unsigned int i = 0; i < data_types.size(); i++)
			{
				for(unsigned int j = 0; j < flw_formats.size(); j++)
				{
					for(unsigned int k = 0; k < flw_formats[j].params.size(); k++)
					{
						if(flw_formats[j].found == false && flw_formats[j].params[k] == data_types[i])
						{
							flw_formats[j].found = true;
							Flow3dData d;
							d.format.type = flw_formats[j].type;
							for(unsigned int l = 0; l < flw_formats[j].params.size(); l++)
							{
								d.format.params.push_back(flw_formats[j].params[l]);
							}
							data.push_back(d);
						}
					}
				}
			}

			frameHeaderExtracted = true;
		}
		
		//Particle data for one frame
		
		int index_count = 0;
		std::vector<unsigned int> indices;
		frame_indices.push_back(indices);

		int *frame_index = new int[data.size()];
		for(unsigned int i = 0; i < data.size(); i++)
		{
			
			frame_index[i] = data[i].frame_data.size();

			std::vector<float> frame;
			data[i].frame_data.push_back(frame);
		}
		while(std::getline(fin, lineread))
		{
			CL_String line_read(lineread.c_str());
			std::vector<CL_String> particle_info = CL_StringHelp::split_text(line_read, " ");
			if(particle_info[0] == "printing")
				break;

			for(unsigned int i = 0; i < data.size(); i++)
			{
				for(unsigned int j = 0; j < data_types.size(); j++)
				{
					bool data_set = false;
					CL_String param_type = data_types[j];
					for(unsigned int k = 0; k < data[i].format.params.size(); k++)
					{
						if(data[i].format.params[k] == param_type)
						{
							float particle_var = CL_StringHelp::text_to_float(particle_info[j]);
							data[i].frame_data[frame_index[i]].push_back(particle_var);
							data_set = true;
							break;
						}
					}

					if(data_set)
						frame_indices[frame_index[0]].push_back(index_count++);
				}
			}
		}
		delete[] frame_index;
	}
 	return true;
}

bool Flow3D::saveBinary()
{
	return true;
}

void Flow3D::calcBufferData()
{
	unsigned int index_count = 0;
	unsigned int num_frames = frame_indices.size();
	frame_vao.resize(num_frames);
	frame_ibo.resize(num_frames);
	frame_vbo.resize(num_frames);
	glGenVertexArrays(num_frames, &frame_vao[0]);

	for(unsigned int frame_index = 0; frame_index < num_frames; frame_index++)
	{
		glBindVertexArray(frame_vao[frame_index]);

		/*unsigned int indSize = sizeof(unsigned int)*frame_indices[frame_index].size();

		glGenBuffers(1, &frame_ibo[frame_index]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frame_ibo[frame_index]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, &frame_indices[frame_index], GL_STATIC_DRAW);*/

		glGenBuffers(1, &frame_vbo[frame_index]);
		glBindBuffer(GL_ARRAY_BUFFER, frame_vbo[frame_index]);

		unsigned int buffer_size = 0;
		for(unsigned int data_index = 0; data_index < data.size(); data_index++)
		{
			buffer_size += data[data_index].frame_data[frame_index].size();
		}
		buffer_size *= sizeof(float);

		glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW);
		shader.setShader(cl_format("resources/Shaders/%1", shaderName.Get()));
		shader.initShader();

		unsigned int buffer_offset = 0;
		for(unsigned int data_index = 0; data_index < data.size(); data_index++)
		{
			unsigned int sub_buffer_size = sizeof(float) * data[data_index].frame_data[frame_index].size();
			glBufferSubData(GL_ARRAY_BUFFER, buffer_offset, sub_buffer_size, &data[data_index].frame_data[frame_index][0]);
			
			//Setup shader attribute for sub_buffer
			CL_String attribName = data[data_index].format.type;
			unsigned int num_params = data[data_index].format.params.size();
			Engine::shaderAttrib(shader.getShaderProg(), attribName.c_str(), num_params, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(buffer_offset));

			//Offset buffer for next iteration of data
			buffer_offset = sub_buffer_size;
		}		
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Flow3D::play()
{
	play_frames = true;
}

void Flow3D::stop()
{
	play_frames = false;
}

void Flow3D::change_frame(int i)
{
	int num_frames = (int)frame_indices.size();
	current_frame += i;
	if(current_frame > num_frames-1)
		current_frame = 0;
	else if(current_frame < 0)
		current_frame = num_frames-1;
}

