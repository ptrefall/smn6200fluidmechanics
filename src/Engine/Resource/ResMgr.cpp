#include "precomp.h"
#include "ResMgr.h"
#include "ResFactory.h"
#include "ResReg.h"

using namespace Engine;

ResMgr::ResMgr(CoreMgr *coreMgr, const CL_String &base_path)
: factory(NULL)
{
	this->coreMgr = coreMgr;
	init(base_path);
}

ResMgr::~ResMgr()
{
	if(factory)
	{
		delete factory;
		factory = NULL;
	}
}

void ResMgr::init(const CL_String &base_path)
{
	vfs = CL_VirtualFileSystem(base_path, false);
	if(vfs.is_null())
		throw CL_Exception("Failed to load virtual file system!");

	vdir = vfs.get_root_directory();

	factory = new ResFactory(coreMgr);
	ResReg::Register(factory);
}

CL_String ResMgr::getRootPath()
{
	return vfs.get_provider()->get_path();
}

std::vector<CL_String> ResMgr::getFilesInDir(const CL_String &dir)
{
	std::vector<CL_String> files;
	CL_VirtualDirectory vdirWithFiles = vdir.open_directory(dir);
	CL_VirtualDirectoryListing vdir_list = vdirWithFiles.get_directory_listing();
	while(vdir_list.next())
	{
		if(vdir_list.is_directory())
			continue;
		if(!vdir_list.is_readable())
			continue;

		files.push_back(vdir_list.get_filename());
	}
	return files;
}

IResource *ResMgr::create(const CL_String &fileName, const CL_String &fileType)
{
	if(factory == NULL)
		return NULL;

	return factory->create(fileName, fileType);
}
