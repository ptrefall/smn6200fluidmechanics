#pragma once

namespace Engine
{
class CoreMgr;
class IResource;

class ResFactory
{
public:
	ResFactory(CoreMgr *coreMgr);
	~ResFactory();

	typedef IResource *(* ResourceCreator)(CoreMgr *coreMgr, const CL_String &fileName);
	void registerResource(const CL_String &fileType, ResourceCreator functor);
	IResource *create(const CL_String &fileName, const CL_String &fileType);

	static std::map<CL_String, ResourceCreator>* creators;

private:
	CoreMgr *coreMgr;

	std::map<CL_String, IResource*> xmlResources;
};

}
