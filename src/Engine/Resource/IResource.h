#pragma once

namespace Engine
{
class CoreMgr;

class IResource
{
public:
	IResource(CoreMgr *coreMgr, const CL_String &fileName) {}
	virtual ~IResource() {}

	virtual CL_String getResType() const = 0;

	virtual int getInt(const CL_String &id) { return -1; }
	virtual bool getBool(const CL_String &id) { return false; }
	virtual CL_String getString(const CL_String &id) { return CL_String(); }

	virtual CL_DataBuffer *getBuffer() const { return 0; }
};

}
