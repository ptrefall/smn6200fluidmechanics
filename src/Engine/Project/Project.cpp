#include "precomp.h"
#include "Project.h"

#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <Entity/IEntity.h>

using namespace Engine;

Project::Project(CoreMgr *coreMgr, const CL_String &name)
: coreMgr(coreMgr), selectedEntity(NULL)
{
	this->name = name;
	coreMgr->getGuiMgr()->setCaptionText(cl_format("Fluid Mechanics\tProject: %1", name).c_str());
}

Project::~Project()
{
}

bool Project::addEntity(IEntity *entity)
{
	entities.push_back(entity);
	unsaved_entities.push_back(entity);
	return true;
}

void Project::selectEntity(IEntity *entity)
{
	selectedEntity = entity;
}

void Project::save()
{
	for(unsigned int i = 0; i < unsaved_entities.size(); i++)
	{
	}
	unsaved_entities.clear();
}
