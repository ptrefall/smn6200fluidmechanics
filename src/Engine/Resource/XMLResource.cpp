#include "precomp.h"
#include "XMLResource.h"
#include "ResMgr.h"

#include <Core/CoreMgr.h>

using namespace Engine;

XMLResource::XMLResource(CoreMgr *coreMgr, const CL_String &fileName)
: IResource(coreMgr, fileName), doc(NULL)
{
	this->coreMgr = coreMgr;

	CL_IODevice file = coreMgr->getResMgr()->getDir().open_file(cl_format("%1/%2", "XML", fileName), CL_File::open_existing, CL_File::access_read);
	doc = new CL_DomDocument(file, false);
	if(doc->is_null())
		throw CL_Exception(cl_format("%1: %2", "Failed to load XML Resource", fileName));
}

XMLResource::~XMLResource()
{
	if(doc)
	{
		delete doc;
		doc = NULL;
	}
}

int XMLResource::getInt(const CL_String &id)
{
	if(doc == NULL)
		return 0;

	return doc->select_int(id);
}

bool XMLResource::getBool(const CL_String &id)
{
	if(doc == NULL)
		return false;

	return doc->select_bool(id);
}

CL_String XMLResource::getString(const CL_String &id)
{
	if(doc == NULL)
		return "";

	return doc->select_string(id);
}
