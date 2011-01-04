#pragma once

#include "IResource.h"

namespace Engine
{
class CoreMgr;

class XMLResource : public IResource
{
public:
	XMLResource(CoreMgr *coreMgr, const CL_String &fileName);
	virtual ~XMLResource();

	static CL_String getType() { return "XML"; }
	static IResource *create(CoreMgr *coreMgr, const CL_String &fileName) { return new XMLResource(coreMgr, fileName); }

	virtual CL_String getResType() const { return getType(); }
	virtual int getInt(const CL_String &id);
	virtual bool getBool(const CL_String &id);
	virtual CL_String getString(const CL_String &id);

	CL_DomDocument &getDoc() { return *doc; }

private:
	CoreMgr *coreMgr;
	CL_DomDocument *doc;
};

}
