#include "precomp.h"
#include "CoreMgr.h"
#include "Cam.h"

#include <Event/IEventManager.h>
#include <Event/EventManager.h>
#include <Gui/GuiMgr.h>
#include <Resource/ResMgr.h>
#include <Script/ScriptMgr.h>
#include <Entity/EntityManager.h>
#include <Entity/IEntity.h>

#include <Scene/Scene.h>
#include <Resource/IResource.h>
#include <WorkThread/WorkThreadMgr.h>

#ifdef WIN32
#include <Core/Win32Timer.h>
#endif

using namespace Engine;

namespace {
	int g_width, g_height;
	bool g_resize;

	void g_resize_cb(int w, int h)
	{
		g_width = w;
		g_height = h;
		g_resize = true;
	}
}

CoreMgr::CoreMgr(const CL_String &base_path)
: setupCore(new CL_SetupCore()),
  eventMgr(NULL), guiMgr(NULL), resMgr(NULL), scriptMgr(NULL), entityMgr(NULL), workThreadMgr(NULL), timer(NULL), cam(NULL), stop(false)
{
	init(base_path);
	run();
}

CoreMgr::~CoreMgr()
{
	if(eventMgr)
	{
		delete eventMgr;
		eventMgr = NULL;
	}
	if(guiMgr)
	{
		delete guiMgr;
		guiMgr = NULL;
	}
	if(resMgr)
	{
		delete resMgr;
		resMgr = NULL;
	}
	if(scriptMgr)
	{
		delete scriptMgr;
		scriptMgr = NULL;
	}
	if(entityMgr)
	{
		delete entityMgr;
		entityMgr = NULL;
	}
	if(workThreadMgr)
	{
		delete workThreadMgr;
		workThreadMgr = NULL;
	}
	if(timer)
	{
		delete timer;
		timer = NULL;
	}
	if(cam)
	{
		delete cam;
		cam = NULL;
	}
}

void CoreMgr::init(const CL_String &base_path)
{
	int fail = 0;

	resMgr = new ResMgr(this, base_path);
	IResource *cfg = resMgr->create("config.xml", "XML");

	int fullscreen = cfg->getBool("Config/GUI/Fullscreen");
	int w = cfg->getInt("Config/GUI/Width");
	int h = cfg->getInt("Config/GUI/Height");
	int d = cfg->getInt("Config/GUI/Depth");
	int vsync = cfg->getInt("Config/GUI/VSync");
	guiMgr = new GuiMgr(this, (fullscreen > 0), w, h, d, vsync);
	guiMgr->addDocument("Main", cl_format("%1Gui/demo.rml", resMgr->getRootPath()));

	cam = new Cam(w,h);
	eventMgr = new Events::EventManager();
	entityMgr = new EntityManager(this);
	scriptMgr = new ScriptMgr(this);
	scriptMgr->init();
	workThreadMgr = new WorkThreadMgr(this);

	CL_String scene_script = cfg->getString("Config/Scene/Script");
	Scene::init_scene(this, scene_script);

	g_width = w;
	g_height = h;
	g_resize = false;
	glfwSetWindowSizeCallback(&g_resize_cb);

	timer = new Win32Timer();
}

void CoreMgr::run()
{
	timer->start();

	//Some random opengl init stuff
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	while(guiMgr->isWindowOpen() && !stop)
	{
		if(g_resize)
		{
			glViewport(0, 0, g_width, g_height);
			cam->perspective(40.0f, (float)g_width/(float)g_height, 1.0f, 1000.0f);

			guiMgr->resize(g_width, g_height);
			g_resize = false;
		}

		float dt = (float)timer->update();
		checkForInput(dt);
		workThreadMgr->update(dt);
		entityMgr->update(dt);
		guiMgr->update(dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		entityMgr->render();
		guiMgr->render();
		guiMgr->swapBuffers();
	}
	timer->stop();
}

void CoreMgr::checkForInput(float dt)
{
	///////////////
	// CHECKING SPECIAL KEYS
	///////////////
	if(glfwGetKey(GLFW_KEY_ESC) == GLFW_PRESS)
	{
		stop = true;
	}
	if(glfwGetKey(GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_SPACE, true);
	}
	if(glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_UP, true);
	}
	else if(glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_DOWN, true);
	}
	if(glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_LEFT, true);
	}
	else if(glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_RIGHT, true);
	}

	///////////////
	// CHECKING ASCII KEYS
	///////////////
	if(glfwGetKey('w') == GLFW_PRESS || glfwGetKey('W') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_W, true);
	}
	else if(glfwGetKey('s') == GLFW_PRESS || glfwGetKey('S') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_S, true);
	}

	if(glfwGetKey('a') == GLFW_PRESS || glfwGetKey('A') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_A, true);
	}
	else if(glfwGetKey('d') == GLFW_PRESS || glfwGetKey('D') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_D, true);
	}

	if(glfwGetKey('f') == GLFW_PRESS || glfwGetKey('F') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_F, true);
	}
	else if(glfwGetKey('v') == GLFW_PRESS || glfwGetKey('V') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_V, true);
	}

	if(glfwGetKey('q') == GLFW_PRESS || glfwGetKey('Q') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_Q, true);
	}
	else if(glfwGetKey('e') == GLFW_PRESS || glfwGetKey('E') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_E, true);
	}
	if(glfwGetKey('z') == GLFW_PRESS || glfwGetKey('Z') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_Z, true);
	}
	else if(glfwGetKey('x') == GLFW_PRESS || glfwGetKey('X') == GLFW_PRESS)
	{
		guiMgr->inject(Rocket::Core::Input::KI_X, true);
	}

	////////////////
	// CHECKING MOUSE BUTTONS
	////////////////
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		guiMgr->injectMouse(0, true);
	}
	else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		guiMgr->injectMouse(0, false);
	}
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		guiMgr->injectMouse(1, true);
	}
	else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		guiMgr->injectMouse(1, false);
	}
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
	{
		guiMgr->injectMouse(2, true);
	}
	else if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
	{
		guiMgr->injectMouse(2, false);
	}

	////////////////
	// CHECKING MOUSE POSITION
	////////////////
	CL_Vec2i mousePos;
	glfwGetMousePos(&mousePos.x, &mousePos.y);
	guiMgr->inject(mousePos);
}

