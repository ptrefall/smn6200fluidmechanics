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
  eventMgr(NULL), guiMgr(NULL), resMgr(NULL), scriptMgr(NULL), entityMgr(NULL), workThreadMgr(NULL), timer(NULL), cam(NULL)
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

	while(guiMgr->isWindowOpen())
	{
		if(g_resize)
		{
			//scene->reshape(g_width, g_height);
			g_resize = false;
		}

		float dt = (float)timer->update();
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
