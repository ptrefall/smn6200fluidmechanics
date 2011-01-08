#include "precomp.h"
#include "ExposeProjectMgr.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Resource/ResMgr.h>

#include <iostream>
#include <sstream>
#include <fstream>

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
}

void ExposeProjectMgr::CreateProject(LuaObject lFilename)
{
	if(!lFilename.IsString())
	{
		CL_String err = cl_format("Failed to create project, because the type of filename was %1 when expecting String!", lFilename.TypeName());
		throw CL_Exception(err);
	}

	CL_String filename = lFilename.ToString();
	CL_String path = cl_format("%1Projects/%2.proj", coreMgr->getResMgr()->getRootPath(), filename);
	
	std::ofstream fout(path.c_str(), std::ios::binary);
	if(fout.bad())
	{
		fout.close();
		return;
	}

	CL_String header_start = cl_format("Project: %1%2", CL_StringHelp::text_to_upper(filename.substr(0,1)), CL_StringHelp::text_to_lower(filename.substr(1,CL_String::npos)));
	fout.write((char*)&header_start, sizeof(header_start));
	fout.close();
}
