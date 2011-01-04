#include "precomp.h"
#include "Scene.h"

#include <Core/CoreMgr.h>
#include <Script/ScriptMgr.h>

using namespace Engine;

void Scene::init_scene(CoreMgr *coreMgr, const CL_String &scene_script)
{
	int failed = coreMgr->getScriptMgr()->doFile(cl_format("Scenes/%1", scene_script));
	if(failed)
		throw CL_Exception(cl_format("Failed to load scene %1", scene_script));
}
