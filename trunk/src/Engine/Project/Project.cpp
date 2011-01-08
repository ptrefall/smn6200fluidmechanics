#include "precomp.h"
#include "Project.h"

#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>

using namespace Engine;

Project::Project(CoreMgr *coreMgr, const CL_String &name)
: coreMgr(coreMgr)
{
	this->name = name;
	coreMgr->getGuiMgr()->setCaptionText(cl_format("Fluid Mechanics\tProject: %1", name).c_str());
}

Project::~Project()
{
}
