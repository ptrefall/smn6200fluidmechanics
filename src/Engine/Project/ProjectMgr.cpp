#include "precomp.h"
#include "ProjectMgr.h"
#include "Project.h"

#include <Core/CoreMgr.h>
#include <Resource/ResMgr.h>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace Engine;

ProjectMgr::ProjectMgr(CoreMgr *coreMgr)
: coreMgr(coreMgr), project(NULL)
{
}

ProjectMgr::~ProjectMgr()
{
	if(project)
	{
		delete project;
		project = NULL;
	}
}

bool ProjectMgr::createProject(const CL_String &filename)
{
	if(project)
		throw CL_Exception("A project has already been loaded, close it before trying to create a new project!");

	CL_String path = cl_format("%1Projects/%2.proj", coreMgr->getResMgr()->getRootPath(), filename);
	
	std::ofstream fout(path.c_str(), std::ios::binary);
	if(fout.bad())
	{
		fout.close();
		CL_Console::write_line(cl_format("Could not create project file %1", path));
		return false;
	}

	CL_String name = cl_format("%1%2", CL_StringHelp::text_to_upper(filename.substr(0,1)), CL_StringHelp::text_to_lower(filename.substr(1,CL_String::npos)));
	project = new Project(coreMgr, name);
	CL_String::size_type name_size = name.size();
	fout.write((char*)&name_size, sizeof(CL_String::size_type));
	fout.write((char*)name.data(), name_size);
	fout.close();

	return true;
}

bool ProjectMgr::loadProject(const CL_String &filename)
{
	if(project)
		throw CL_Exception("A project has already been loaded, close it before trying to load another project!");

	CL_String path = cl_format("%1Projects/%2.proj", coreMgr->getResMgr()->getRootPath(), filename);
	
	std::ifstream fin(path.c_str(), std::ios::binary);
	if(fin.bad())
	{
		fin.close();
		CL_Console::write_line(cl_format("Could not find/open project file %1", path));
		return false;
	}

	CL_String name;
	CL_String::size_type name_size;
	fin.read((char*)&name_size, sizeof(CL_String::size_type));
	name.resize(name_size);
	fin.read((char*)name.data(), name_size);
	project = new Project(coreMgr, name);
	
	fin.close();
	return true;
}