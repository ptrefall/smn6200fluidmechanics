#pragma once

namespace Engine
{
class CoreMgr;
class IResource;
class ResFactory;
class ResMgr
{
public:
	ResMgr(CoreMgr *coreMgr, const CL_String &base_path);
	~ResMgr();

	CL_String getRootPath();
	std::vector<CL_String> getFilesInDir(const CL_String &dir);

	CL_VirtualDirectory &getDir() { return vdir; }

	IResource *create(const CL_String &fileName, const CL_String &fileType);

private:
	void init(const CL_String &base_path);

	CL_VirtualFileSystem vfs;
	CL_VirtualDirectory vdir;

	CoreMgr *coreMgr;
	ResFactory *factory;
};

}
