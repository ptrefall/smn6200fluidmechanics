#include "precomp.h"
#include "ExposeProjectMgr.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Project/ProjectMgr.h>

using namespace Engine;
using namespace LuaPlus;

ExposeProjectMgr::ExposeProjectMgr(CoreMgr *coreMgr)
{
	this->coreMgr = coreMgr;
	init();
}

ExposeProjectMgr::~ExposeProjectMgr()
{
}

void ExposeProjectMgr::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("CreateProject", *this, &ExposeProjectMgr::CreateProject);
	globals.RegisterDirect("LoadProject", *this, &ExposeProjectMgr::LoadProject);
}

LuaObject ExposeProjectMgr::CreateProject(LuaObject lFilename)
{
	if(!lFilename.IsString())
	{
		CL_String err = cl_format("Failed to create project, because the type of filename was %1 when expecting String!", lFilename.TypeName());
		throw CL_Exception(err);
	}

	CL_String filename = lFilename.ToString();
	bool success = coreMgr->getProjectMgr()->createProject(filename);
	LuaObject lSuccess;
	lSuccess.AssignBoolean(coreMgr->getScriptMgr()->GetGlobalState()->Get(), success);
	return lSuccess;
}

LuaObject ExposeProjectMgr::LoadProject(LuaObject lFilename)
{
	if(!lFilename.IsString())
	{
		CL_String err = cl_format("Failed to create project, because the type of filename was %1 when expecting String!", lFilename.TypeName());
		throw CL_Exception(err);
	}

	CL_String filename = lFilename.ToString();
	bool success = coreMgr->getProjectMgr()->loadProject(filename);
	LuaObject lSuccess;
	lSuccess.AssignBoolean(coreMgr->getScriptMgr()->GetGlobalState()->Get(), success);
	return lSuccess;
}
