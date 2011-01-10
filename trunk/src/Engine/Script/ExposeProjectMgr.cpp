#include "precomp.h"
#include "ExposeProjectMgr.h"
#include "ExposeEntityManager.h"
#include "ExposeIEntity.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Project/ProjectMgr.h>
#include <Entity/IEntity.h>


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
	globals.RegisterDirect("AddEntity", *this, &ExposeProjectMgr::AddEntity);
	globals.RegisterDirect("SelectEntity", *this, &ExposeProjectMgr::SelectEntity);
	globals.RegisterDirect("SaveProject", *this, &ExposeProjectMgr::SaveProject);
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

LuaObject ExposeProjectMgr::AddEntity(LuaObject lEntity)
{
	if(!lEntity.IsTable())
	{
		CL_String err = cl_format("Failed to add entity to project, because the type of entity was %1 when expecting Table!", lEntity.TypeName());
		throw CL_Exception(err);
	}

	unsigned int id = lEntity.GetByName("id").ToInteger();
	IEntity *entity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getExposedEntity(id)->getEntity();
	bool success = coreMgr->getProjectMgr()->addEntity(entity);
	LuaObject lSuccess;
	lSuccess.AssignBoolean(coreMgr->getScriptMgr()->GetGlobalState()->Get(), success);
	return lSuccess;
}

void ExposeProjectMgr::SelectEntity(LuaPlus::LuaObject lEntity)
{
	if(!lEntity.IsTable())
	{
		CL_String err = cl_format("Failed to add entity to project, because the type of entity was %1 when expecting Table!", lEntity.TypeName());
		throw CL_Exception(err);
	}

	unsigned int id = lEntity.GetByName("id").ToInteger();
	IEntity *entity = coreMgr->getScriptMgr()->getExposedEntityMgr()->getExposedEntity(id)->getEntity();
	coreMgr->getProjectMgr()->selectEntity(entity);
}

void ExposeProjectMgr::SaveProject()
{
	coreMgr->getProjectMgr()->save();
}
