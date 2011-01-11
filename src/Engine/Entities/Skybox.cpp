#include "precomp.h"
#include "Skybox.h"
#include <Event/Event.h>
#include <Event/EventValue.h>
#include <Core/CoreMgr.h>
#include <Core/Cam.h>
#include <Resource/ResMgr.h>
#include <Resource/TexResource.h>

using namespace Engine;

Skybox::Skybox(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory)
: IEntity(id, type, name, coreMgr, factory), solid(true), size(1000.0f), shouldUpdate(false)
{
	pos = this->AddProperty<CL_Vec3f>("Position", CL_Vec3f(0,0,0));
	rot = this->AddProperty<CL_Mat3f>("RotationMatrix", CL_Mat3f::identity());
	
	pitch = this->AddProperty<float>("Pitch", 0.0f);
	pitchRate = this->AddProperty<float>("PitchRate", 1.0f);
	
	yaw = this->AddProperty<float>("Yaw", 0.0f);
	yawRate = this->AddProperty<float>("YawRate", 1.0f);
	
	slotPitchChanged = pitch.ValueChanged().connect(this, &Skybox::OnPitchChanged);
	slotYawChanged = yaw.ValueChanged().connect(this, &Skybox::OnYawChanged);

	upDirection = CL_Vec3f(0.0f, 1.0f, 0.0f);

	TexResource::beginTextureCube(texture);
	TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "X+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "X-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Y+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Y-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Z+.tga").c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
    TexResource::createTextureCube( cl_format("%1Skybox/%2", coreMgr->getResMgr()->getRootPath(), "Z-.tga").c_str(), GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
	TexResource::endTextureCube(texture);

	initIndices();
	initVertices();
	initTexCoords();

	unsigned int vertSize = sizeof(float)*vertices.size();
	unsigned int indSize = sizeof(unsigned int)*indices.size();
	unsigned int texSize = sizeof(float)*texCoords.size();

	unsigned int vertOffset = 0;
	unsigned int indOffset = 0;
	unsigned int texOffset = vertOffset + texSize;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize, &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertSize+texSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, vertOffset, vertSize, &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, texOffset, texSize, &texCoords[0]);

	shader.setShader("resources/Shaders/skybox");
	shader.initShader();
	Engine::shaderAttrib(shader.getShaderProg(), "vVertex", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertOffset));
	Engine::shaderAttrib(shader.getShaderProg(), "vTexCoord", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(texOffset));

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Skybox::~Skybox()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Skybox::Update(float dt)
{
	if(shouldUpdate)
	{
		updateMatrix(dt);
		shouldUpdate = false;
	}
}

void Skybox::Render()
{
	if(alpha.Get() == 0.0f)
		return;

	if(scale.Get() == 0.0f)
		return;

	glDisable( GL_DEPTH_TEST );
    glDepthMask(GL_FALSE);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

	shader.enableShader();
	{
		bindTexture();
		bindUniforms();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindVertexArray(0);
		unbindTexture();
	} shader.disableShader();

	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	glEnable( GL_DEPTH_TEST );
}

void Skybox::bindUniforms()
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

	int loc = glGetUniformLocation(shader.getShaderProg(), "projMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &projMat[0]);
	
	loc = glGetUniformLocation(shader.getShaderProg(), "mvMat");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mvMat[0]);

	loc = glGetUniformLocation(shader.getShaderProg(), "fCube");
	if(loc < 0)
		throw CL_Exception("LOC was -1");
	glUniform1i(loc, 0);
}

void Skybox::bindTexture()
{
	glEnable(GL_TEXTURE_CUBE_MAP);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void Skybox::unbindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDisable(GL_TEXTURE_CUBE_MAP);
}

void Skybox::initIndices()
{
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(2); indices.push_back(3); indices.push_back(0);

	indices.push_back(4); indices.push_back(5); indices.push_back(6);
	indices.push_back(6); indices.push_back(7); indices.push_back(4);

	indices.push_back(8); indices.push_back(9); indices.push_back(10);
	indices.push_back(10); indices.push_back(11); indices.push_back(8);

	indices.push_back(12); indices.push_back(13); indices.push_back(14);
	indices.push_back(14); indices.push_back(15); indices.push_back(12);

	indices.push_back(16); indices.push_back(17); indices.push_back(18);
	indices.push_back(18); indices.push_back(19); indices.push_back(16);

	indices.push_back(20); indices.push_back(21); indices.push_back(22);
	indices.push_back(22); indices.push_back(23); indices.push_back(20);
}

void Skybox::initVertices()
{
	//BACK
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);

	//BOTTOM
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);

	//FRONT
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);

	//LEFT
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);

	//RIGHT
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);

	//TOP
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
}

void Skybox::initTexCoords()
{
	//BACK
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(-1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(-1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(1.0f); texCoords.push_back(-1.0f);

	//BOTTOM
	texCoords.push_back(-1.0f); texCoords.push_back(-1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(-1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(-1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(-1.0f); texCoords.push_back(1.0f);

	//FRONT
	texCoords.push_back(-1.0f); texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(-1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(-1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(1.0f);

	//LEFT
	texCoords.push_back(-1.0f); texCoords.push_back(-1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(-1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(1.0f); texCoords.push_back(-1.0f);

	//RIGHT
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(-1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(-1.0f); texCoords.push_back(-1.0f);

	//BACK
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(1.0f); texCoords.push_back(-1.0f);
	texCoords.push_back(-1.0f); texCoords.push_back(1.0f); texCoords.push_back(1.0f);
}

void Skybox::updateMatrix(float dt)
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

void Skybox::OnPitchChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}

void Skybox::OnYawChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}
