#include "precomp.h"
#include "StlMesh.h"
#include <Event/Event.h>
#include <Event/EventValue.h>
#include <Core/CoreMgr.h>
#include <Core/Cam.h>
#include <Resource/ResMgr.h>
#include <Resource/TexResource.h>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace Engine;

StlMesh::StlMesh(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory)
: IEntity(id, type, name, coreMgr, factory), solid(true), size(4.0f), shouldUpdate(false), loaded(false)
{
	pos = this->AddProperty<CL_Vec3f>("Position", CL_Vec3f(0,0,0));
	rot = this->AddProperty<CL_Mat3f>("RotationMatrix", CL_Mat3f::identity());
	
	pitch = this->AddProperty<float>("Pitch", 0.0f);
	pitchRate = this->AddProperty<float>("PitchRate", 1.0f);
	
	yaw = this->AddProperty<float>("Yaw", 0.0f);
	yawRate = this->AddProperty<float>("YawRate", 1.0f);

	mesh = this->AddProperty<CL_String>("Mesh", CL_String());
	
	slotPitchChanged = pitch.ValueChanged().connect(this, &StlMesh::OnPitchChanged);
	slotYawChanged = yaw.ValueChanged().connect(this, &StlMesh::OnYawChanged);
	slotMeshChanged = mesh.ValueChanged().connect(this, &StlMesh::OnMeshChanged);

	upDirection = CL_Vec3f(0.0f, 1.0f, 0.0f);

	TexResource::beginTextureCube(texture);
	TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "X+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "X-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Y+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Y-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Z+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Z-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	TexResource::endTextureCube(texture);

}

StlMesh::~StlMesh()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

bool StlMesh::load(const CL_String &filename)
{
	if(loaded)
		return false;

	CL_String path = cl_format("%1Mesh/%2.stl", coreMgr->getResMgr()->getRootPath(), filename);

	//Open file
	std::ifstream fin(path.c_str(), std::ios::binary);
	if(fin.bad())
	{
		fin.close();
		CL_Console::write_line(cl_format("Could not open stl mesh file %1", path));
		return false;
	}

	//Header
	fin.read((char*)&stl_header.head, sizeof(unsigned char)*80);
	fin.read((char*)&stl_header.num_tris, sizeof(unsigned long int));

	//Triangles
	stl_tris = new StlTri[stl_header.num_tris];
	for(unsigned long int i = 0; i < stl_header.num_tris; i++)
	{
		fin.read((char*)&stl_tris[i], sizeof(float)*12+sizeof(unsigned short));
	}

	//Close file
	fin.close();

	calcBufferData();

	delete[] stl_tris;

	return true;
}

void StlMesh::calcBufferData()
{
	unsigned int index_count = 0;

	for(unsigned long int i = 0; i < stl_header.num_tris; i++)
	{
		indices.push_back(index_count++); indices.push_back(index_count++); indices.push_back(index_count++);

		StlTri &tri = stl_tris[i];
		vertices.push_back(tri.vert1[0]*50.0f); vertices.push_back(tri.vert1[2]*50.0f); vertices.push_back(tri.vert1[1]*50.0f);
		vertices.push_back(tri.vert2[0]*50.0f); vertices.push_back(tri.vert2[2]*50.0f); vertices.push_back(tri.vert2[1]*50.0f);
		vertices.push_back(tri.vert3[0]*50.0f); vertices.push_back(tri.vert3[2]*50.0f); vertices.push_back(tri.vert3[1]*50.0f);

		normals.push_back(tri.norm[0]); normals.push_back(tri.norm[2]); normals.push_back(tri.norm[1]);
		normals.push_back(tri.norm[0]); normals.push_back(tri.norm[2]); normals.push_back(tri.norm[1]);
		normals.push_back(tri.norm[0]); normals.push_back(tri.norm[2]); normals.push_back(tri.norm[1]);
	}


	unsigned int vertSize = sizeof(float)*vertices.size();
	unsigned int indSize = sizeof(unsigned int)*indices.size();
	unsigned int normSize = sizeof(float)*normals.size();

	unsigned int vertOffset = 0;
	unsigned int indOffset = 0;
	unsigned int normOffset = vertOffset + vertSize;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize+normSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, vertOffset, vertSize, &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, normOffset, normSize, &normals[0]);

	shader.setShader("resources/Shaders/stl");
	shader.initShader();
	Engine::shaderAttrib(shader.getShaderProg(), "vVertex", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertOffset));
	Engine::shaderAttrib(shader.getShaderProg(), "vNormal", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normOffset));

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void StlMesh::Update(float dt)
{
	if(!loaded)
		return;

	if(shouldUpdate)
	{
		updateMatrix(dt);
		shouldUpdate = false;
	}
}

void StlMesh::Render()
{
	if(!loaded)
		return;

	shader.enableShader();
	{
		bindTexture();
		bindUniforms();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindVertexArray(0);
		unbindTexture();
	} shader.disableShader();
}

void StlMesh::bindUniforms()
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
}

void StlMesh::bindTexture()
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void StlMesh::unbindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void StlMesh::updateMatrix(float dt)
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

void StlMesh::OnPitchChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}

void StlMesh::OnYawChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}

void StlMesh::OnMeshChanged(const CL_String &oldValue, const CL_String &newValue)
{
	if(!load(newValue))
		throw CL_Exception(cl_format("Failed to load STL mesh %1", newValue));

	loaded = true;
}
