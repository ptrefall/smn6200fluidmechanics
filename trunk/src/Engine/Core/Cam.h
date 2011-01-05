#pragma once

namespace Engine
{
	class Cam
	{
	public:
		Cam(const int &width, const int &height);
		~Cam();

		const CL_Mat4f &perspective(float fovx, float aspect, float znear, float zfar);

		const CL_Mat4f &getViewMatrix() const { return viewMatrix; }
		const CL_Mat4f &getProjectionMatrix() const { return projMatrix; }
		
		void moveLeft(float dt);
		void moveRight(float dt);
		void moveForward(float dt);
		void moveBackward(float dt);
		void moveUp(float dt);
		void moveDown(float dt);
		
		void pitchUp(float dt);
		void pitchDown(float dt); 
		void yawRight(float dt);
		void yawLeft(float dt);

		void setPos(const CL_Vec3f& pos) { last_pos = this->pos; this->pos = pos; velocity = CL_Vec3f(0.0f, 0.0f, 0.0f); updateMatrix(true); }
		const CL_Vec3f &getPos() { return pos; }
		const CL_Vec3f &getLastPos() { return last_pos; }

		const CL_Vec3f &getDir() { return direction; }
		const CL_Vec3f &getFwdDir() { return fowardDirection; }
		const CL_Vec3f &getSideDir() { return leftDirection; }
		const CL_Vec3f &getUpDir() { return upDirection; }

		void setFwdSpeed(float speed) { forwardSpeed = speed; }
		void setSideSpeed(float speed) { sidewaysSpeed = speed; }

		
	private:
		void move(float dt, const CL_Vec3f& velocity);
		void updateMatrix(bool move = false);

		CL_Mat4f viewMatrix;
		CL_Mat4f projMatrix;

		CL_Vec3f pos, last_pos;
		float forwardSpeed;
		float sidewaysSpeed;
		float verticalSpeed;

		float pitch;
		float pitchRate;
		float heading;
		float headingRate;
		CL_Quaternionf qPitch;
		CL_Quaternionf qHeading;

		CL_Vec3f direction;
		CL_Vec3f fowardDirection;
		CL_Vec3f leftDirection;
		CL_Vec3f upDirection;
		CL_Vec3f velocity;
	};
}
