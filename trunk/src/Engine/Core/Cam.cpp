#include "precomp.h"
#include "Cam.h"

using namespace Engine;

Cam::Cam(const int &width, const int &height)
{
	viewMatrix = CL_Mat4f::identity();
	perspective(40.0f, (float)width/(float)height, 1.0f, 100000.0f);

	forwardSpeed = 5.0f;
	sidewaysSpeed = 3.0f;
	verticalSpeed = 40.0f;

	pitch = 0.0f;
	pitchRate = 1.0f;
	heading = 0.0f;
	headingRate = 1.0f;

	upDirection = CL_Vec3f(0.0f, 1.0f, 0.0f);
}

Cam::~Cam()
{
}

const CL_Mat4f &Cam::perspective(float fovY, float aspect, float zNear, float zFar)
{
	projMatrix = CL_Mat4f::perspective(fovY, aspect, zNear, zFar);
	return projMatrix;
}

void Cam::updateMatrix(bool move)
{
	CL_Mat4f mat;
	CL_Quaternionf q;
	qPitch = CL_Quaternionf::axis_angle(CL_Angle(pitch, cl_radians), CL_Vec3f(1.0f, 0.0f, 0.0f));
	qHeading = CL_Quaternionf::axis_angle(CL_Angle(heading, cl_radians), CL_Vec3f(0.0f, 1.0f, 0.0f));
	
	q = qPitch * qHeading;
	viewMatrix = q.to_matrix();
	
	mat = qPitch.to_matrix();
	fowardDirection.y = mat[9];

	q = qHeading * qPitch;
	mat = q.to_matrix();
	fowardDirection.x = mat[8];
	fowardDirection.z = mat[10];
	leftDirection = CL_Vec3f::cross(fowardDirection, upDirection);
	if(move)
	{
		float y = pos.y;

		//Define forward direction vector
		if(velocity.z)
		{
			fowardDirection.x *= -velocity.z;
			fowardDirection.y *= -velocity.z;
			fowardDirection.z *= velocity.z;
			direction = fowardDirection;
			//CL_Console::write_line("Move Forward");
		}
		else if(velocity.x)
		{
			//Define sideway direction vector
			leftDirection.x *= -velocity.x;
			leftDirection.y *= -velocity.x;
			leftDirection.z *= velocity.x;
			direction = leftDirection;
			//CL_Console::write_line("Move Sideways");
		}
		else if(velocity.y)
		{
			y += -velocity.y;
			direction = CL_Vec3f(0.0f, 0.0f, 0.0f);
			//CL_Console::write_line("Move Up");
		}
		else
		{
			direction = CL_Vec3f(0.0f, 0.0f, 0.0f);
		}
		
		last_pos = pos;
		pos += direction;
		pos.y = y;
	}
	viewMatrix.translate_self(pos.x, pos.y, pos.z);
}

void Cam::move(float dt, const CL_Vec3f& velocity)
{
	this->velocity.x = velocity.x*sidewaysSpeed*dt;
	this->velocity.y = velocity.y*verticalSpeed*dt;
	this->velocity.z = velocity.z*forwardSpeed*dt;
	updateMatrix(true);
}

void Cam::moveLeft(float dt) { move(dt, CL_Vec3f(2.0f, 0.0f, 0.0f));}
void Cam::moveRight(float dt) {	move(dt, CL_Vec3f(-2.0f, 0.0f, 0.0f));}
void Cam::moveForward(float dt) { move(dt, CL_Vec3f(0.0f, 0.0f, 1.0f));}
void Cam::moveBackward(float dt) { move(dt, CL_Vec3f(0.0f, 0.0f, -1.0f));}
void Cam::moveUp(float dt) { move(dt, CL_Vec3f(0.0f, 1.0f, 0.0f));}
void Cam::moveDown(float dt) { move(dt, CL_Vec3f(0.0f, -1.0f, 0.0f));}
void Cam::pitchUp(float dt)	{ pitch += pitchRate*dt; updateMatrix(); }
void Cam::pitchDown(float dt) { pitch -= pitchRate*dt; updateMatrix(); }
void Cam::yawRight(float dt) { heading += headingRate*dt; updateMatrix(); }
void Cam::yawLeft(float dt) { heading -= headingRate*dt; updateMatrix(); }
