#include "precomp.h"
#include "Volume.h"
#include <Event/Event.h>
#include <Event/EventValue.h>
#include <Core/CoreMgr.h>
#include <Core/Cam.h>

using namespace Engine;

Volume::Volume(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory)
: IEntity(id, type, name, coreMgr, factory), solid(true), size(4.0f), shouldUpdate(false)
{
	pos = this->AddProperty<CL_Vec3f>("Position", CL_Vec3f(0,0,0));
	rot = this->AddProperty<CL_Mat3f>("RotationMatrix", CL_Mat3f::identity());
	
	pitch = this->AddProperty<float>("Pitch", 0.0f);
	pitchRate = this->AddProperty<float>("PitchRate", 1.0f);
	
	yaw = this->AddProperty<float>("Yaw", 0.0f);
	yawRate = this->AddProperty<float>("YawRate", 1.0f);
	
	slotPitchChanged = pitch.ValueChanged().connect(this, &Volume::OnPitchChanged);
	slotYawChanged = yaw.ValueChanged().connect(this, &Volume::OnYawChanged);

	upDirection = CL_Vec3f(0.0f, 1.0f, 0.0f);

	initIndices();
	initVertices();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*vertices.size(), &vertices[0]);

	shader.setShader("resources/Shaders/minimal");
	shader.initShader();
	Engine::shaderAttrib(shader.getShaderProg(), "vVertex", 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Volume::~Volume()
{
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Volume::Update(float dt)
{
	if(shouldUpdate)
	{
		updateMatrix(dt);
		shouldUpdate = false;
	}
}

void Volume::Render()
{
	if(alpha.Get() == 0.0f)
		return;

	if(scale.Get() == 0.0f)
		return;

	shader.enableShader();
	{
		bindUniforms();
		glBindVertexArray(vao);
		if(solid)
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		else
			glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		glBindVertexArray(0);
	} shader.disableShader();
}

void Volume::bindUniforms()
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
}

void Volume::initIndices()
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

void Volume::initVertices()
{
	//TOP
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);

	//BOTTOM
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);

	//FRONT
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);

	//BACK
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);

	//LEFT
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(size);
	vertices.push_back(size); vertices.push_back(size); vertices.push_back(-size);

	//RIGHT
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(size);
	vertices.push_back(-size); vertices.push_back(-size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(-size);
	vertices.push_back(-size); vertices.push_back(size); vertices.push_back(size);
}

void Volume::updateMatrix(float dt)
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

void Volume::OnPitchChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}

void Volume::OnYawChanged(const float &oldValue, const float &newValue)
{
	shouldUpdate = true;
}
