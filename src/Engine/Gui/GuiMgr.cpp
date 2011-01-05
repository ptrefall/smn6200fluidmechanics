#include "precomp.h"
#include "GuiMgr.h"
#include "GuiSystemInterface.h"
#include "GuiRenderInterface.h"

#include <Core/CoreMgr.h>
#include <Resource/ResMgr.h>

#include <iostream>

using namespace Engine;

GuiMgr::GuiMgr(CoreMgr *coreMgr, const bool &fullscr, const int &width, const int &height, const int &depth, const int &vsync)
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


	/////////////////////////////////////////
	// INITIALIZE ROCKET GUI LIBRARY
	/////////////////////////////////////////

	system = new GuiSystemInterface(coreMgr);
	renderer = new GuiRenderInterface(coreMgr);

	Rocket::Core::SetSystemInterface(system);
	Rocket::Core::SetRenderInterface(renderer);

	bool success = Rocket::Core::Initialise();
	if(!success)
		throw CL_Exception("Failed to initialize Rocket GUI Library!");

	addContext("Main", width, height);

	//Add some fonts
	std::vector<CL_String> fonts = coreMgr->getResMgr()->getFilesInDir("/Fonts/");
	for(unsigned int i = 0; i < fonts.size(); i++)
	{
		addFont(cl_format("%1Fonts/%2", coreMgr->getResMgr()->getRootPath(), fonts[i]));
	}
}

GuiMgr::~GuiMgr()
{
	for(unsigned int i = 0; i < contexts.size(); i++)
		contexts[i]->RemoveReference();
	Rocket::Core::Shutdown();

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

void GuiMgr::update(float dt)
{
	for(unsigned int i = 0; i < contexts.size(); i++)
		contexts[i]->Update();
}

void GuiMgr::render()
{
	for(unsigned int i = 0; i < contexts.size(); i++)
		contexts[i]->Render();
}

void GuiMgr::addContext(const CL_String &name, const int &width, const int &height)
{
	Rocket::Core::Context *context = Rocket::Core::CreateContext(name.c_str(), Rocket::Core::Vector2i(width, height));
	contexts.push_back(context);
}

void GuiMgr::addDocument(const CL_String &context_name, const CL_String &path)
{
	Rocket::Core::Context *context = NULL;
	for(unsigned int i = 0; i < contexts.size(); i++)
	{
		if(contexts[i]->GetName() == Rocket::Core::String(context_name.c_str()))
		{
			context = contexts[i];
			break;
		}
	}

	if(context == NULL)
		throw CL_Exception(cl_format("Failed to find context %1", context_name));

	Rocket::Core::ElementDocument* document = context->LoadDocument(path.c_str());
	if (document == NULL)
		throw CL_Exception(cl_format("Failed to load document %1", path));

	document->Show();
	documents.push_back(document);
}

void GuiMgr::addFont(const CL_String &path)
{
	Rocket::Core::FontDatabase::LoadFontFace(path.c_str());
}
