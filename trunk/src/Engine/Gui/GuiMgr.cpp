#include "precomp.h"
#include "GuiMgr.h"
#include "GuiSystemInterface.h"
#include "GuiRenderInterface.h"

#include <Core/CoreMgr.h>
#include <Resource/ResMgr.h>

#include <iostream>

using namespace Engine;

GuiMgr::GuiMgr(CoreMgr *coreMgr, const bool &fullscr, const int &width, const int &height, const int &depth, const int &vsync)
: coreMgr(coreMgr), key_modifier_state(0)
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

	setCaptionText("Fluid Mechanics!!");

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		const unsigned char *errString = glewGetErrorString(err);
		CL_String error = cl_format("Error loading GLEW: %1", (const char*)errString);
		throw CL_Exception(error);
	}

	glfwSwapInterval(vsync);

	orthoMatrix = CL_Mat4f::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f) ;

	/////////////////////////////////////////
	// INITIALIZE ROCKET GUI LIBRARY
	/////////////////////////////////////////

	system = new GuiSystemInterface(coreMgr);
	renderer = new GuiRenderInterface(coreMgr, orthoMatrix);

	Rocket::Core::SetSystemInterface(system);
	Rocket::Core::SetRenderInterface(renderer);

	bool success = Rocket::Core::Initialise();
	if(!success)
		throw CL_Exception("Failed to initialize Rocket GUI Library!");

	addContext("Main", width, height);

#ifdef _DEBUG
	success = Rocket::Debugger::Initialise(contexts[0]);
	if(!success)
		throw CL_Exception("Failed to initialize Rocket GUI Debugger!");

	//Rocket::Debugger::SetVisible(true);
#endif

	//Add some fonts
	std::vector<CL_String> fonts = coreMgr->getResMgr()->getFilesInDir("/Fonts/");
	for(unsigned int i = 0; i < fonts.size(); i++)
	{
		addFont(cl_format("%1Fonts/%2", coreMgr->getResMgr()->getRootPath(), fonts[i]));
	}

	glfwDisable(GLFW_MOUSE_CURSOR);
	loadCursor(cl_format("%1Gui/%2", coreMgr->getResMgr()->getRootPath(), "cursor.rml"));
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

void GuiMgr::resize(int w, int h)
{
	orthoMatrix = CL_Mat4f::ortho(0.0f, (float)w, (float)h, 0.0f, -1.0f, 1.0f) ;
	
	/*Rocket::Core::Vector2i newDim = Rocket::Core::Vector2i(w,h);
	const Rocket::Core::Vector2i &oldDim = contexts[0]->GetDimensions();
	Rocket::Core::Vector2i deltaDim = oldDim - newDim;

	for(unsigned int i = 1; i < contexts.size(); i++)
	{
		contexts[i]->SetDimensions(contexts[i]->GetDimensions()+deltaDim);
	}
	contexts[0]->SetDimensions(newDim);*/
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

void GuiMgr::loadCursor(const CL_String &path)
{
	Rocket::Core::ElementDocument* cursor = contexts[0]->LoadMouseCursor(path.c_str());
	if(cursor == NULL)
		throw CL_Exception(cl_format("Failed to load cursor %1", path));

	cursor->Show();
	documents.push_back(cursor);
}

void GuiMgr::inject(const unsigned int &key, bool state)
{
	if(state)
	{
		contexts[0]->ProcessKeyDown((Rocket::Core::Input::KeyIdentifier)key, key_modifier_state);
	}
	else
	{
		contexts[0]->ProcessKeyUp((Rocket::Core::Input::KeyIdentifier)key, key_modifier_state);
	}
}

void GuiMgr::inject(const CL_Vec2i &mouse_pos)
{
	lastMousePos = mousePos;
	mousePos = mouse_pos;

	contexts[0]->ProcessMouseMove(mousePos.x, mousePos.y, key_modifier_state);
}

void GuiMgr::injectMouse(const int &button, bool state)
{
	if(state)
	{
		contexts[0]->ProcessMouseButtonDown(button, key_modifier_state);
	}
	else
	{
		contexts[0]->ProcessMouseButtonUp(button, key_modifier_state);
	}
}

void GuiMgr::keyModifier(const int &mod, bool state)
{
}
