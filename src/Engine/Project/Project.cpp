#include "precomp.h"
#include "Project.h"

#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <Entity/IEntity.h>
#include <Entities/StlMesh.h>
#include <Entities/Flow3D.h>

using namespace Engine;

Project::Project(CoreMgr *coreMgr, const CL_String &name)
: coreMgr(coreMgr), selectedEntity(NULL)
{
	this->name = name;
	coreMgr->getGuiMgr()->setCaptionText(cl_format("Fluid Mechanics       Project: %1", name).c_str());
}

Project::~Project()
{
}

bool Project::addEntity(IEntity *entity)
{
	if(entity->getSpecialType() == StlMesh::GetStaticSpecialType())
	{
		stl_meshes.push_back(entity);
		unsaved_stl_meshes.push_back(entity);
	}
	else if(entity->getSpecialType() == Flow3D::GetStaticSpecialType())
	{
		flw_sims.push_back(entity);
		unsaved_flw_sims.push_back(entity);
	}
	else
	{
		misc_entities.push_back(entity);
		unsaved_misc_entities.push_back(entity);
	}
	return true;
}

void Project::selectEntity(IEntity *entity)
{
	selectedEntity = entity;
}

void Project::PlayProjectAnimation()
{
	for(unsigned int i = 0; i < flw_sims.size(); i++)
	{
		static_cast<Flow3D*>(flw_sims[i])->play();
	}
}

void Project::StopProjectAnimation()
{
	for(unsigned int i = 0; i < flw_sims.size(); i++)
	{
		static_cast<Flow3D*>(flw_sims[i])->stop();
	}
}

void Project::RewindProjectAnimation()
{
	for(unsigned int i = 0; i < flw_sims.size(); i++)
	{
		static_cast<Flow3D*>(flw_sims[i])->change_frame(-1);
	}
}

void Project::ForwardProjectAnimation()
{
	for(unsigned int i = 0; i < flw_sims.size(); i++)
	{
		static_cast<Flow3D*>(flw_sims[i])->change_frame(1);
	}
}

void Project::save()
{
	for(unsigned int i = 0; i < unsaved_stl_meshes.size(); i++)
	{
	}
	unsaved_stl_meshes.clear();

	for(unsigned int i = 0; i < unsaved_flw_sims.size(); i++)
	{
	}
	unsaved_flw_sims.clear();

	for(unsigned int i = 0; i < unsaved_misc_entities.size(); i++)
	{
	}
	unsaved_misc_entities.clear();
}
