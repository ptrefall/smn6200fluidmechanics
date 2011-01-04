#include "precomp.h"
#include "GuiMgr.h"

#include <iostream>

using namespace Engine;

GuiMgr::GuiMgr(const bool &fullscr, const int &width, const int &height, const int &depth, const int &vsync)
{
	if( !glfwInit() )
        throw CL_Exception("Error initializing glfw");
	
	int w = (int)GLFW_WINDOW;
	if(fullscr)
		w = (int)GLFW_FULLSCREEN;

	//Open OpenGL window
    if( !glfwOpenWindow(width,height, 0,0,0,0, depth,0, w) )
    {
        glfwTerminate();
        throw CL_Exception("Error opening glfw window");
    }

	setCaptionText("GeoMod2!!");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		const unsigned char *errString = glewGetErrorString(err);
		CL_String error = cl_format("Error loading GLEW: %1", (const char*)errString);
		throw CL_Exception(error);
	}

	glfwSwapInterval(vsync);
}

GuiMgr::~GuiMgr()
{
	glfwTerminate();
}

void GuiMgr::swapBuffers()
{
	glfwSwapBuffers();
}

bool GuiMgr::isWindowOpen() const
{
	return (glfwGetWindowParam(GLFW_OPENED) != 0);
}

int GuiMgr::getWidth() const
{
	int width, height;
	glfwGetWindowSize(&width, &height); 
	return width;
}

int GuiMgr::getHeight() const
{
	int width, height;
	glfwGetWindowSize(&width, &height); 
	return height;
}

void GuiMgr::setCaptionText(const char *text)
{
	glfwSetWindowTitle(text);
}
