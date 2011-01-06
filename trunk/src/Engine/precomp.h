#pragma once

#include <GL/glew.h>
#ifdef WIN32
	#include <GL/wglew.h>
#endif
#include <GL/glfw.h>

#include <ClanLib/src/API/core.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>
#include <map>
#include <vector>

#ifndef BUFFER_OFFSET
	#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + bytes)
#endif

namespace Engine
{
	inline void shaderAttrib(unsigned int prog, const char *attribName, int size, GLenum type, bool normalized, int stride, void* pointer)
	{
		int id = glGetAttribLocation(prog, attribName);
		if(id < 0)
			throw CL_Exception(cl_format("Failed to load attribute %1", attribName));
		
		glVertexAttribPointer(id, size, type, normalized, stride, pointer);
		glEnableVertexAttribArray(id);
	}
}
