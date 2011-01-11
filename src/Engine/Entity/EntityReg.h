#pragma once

#include "EntityFactory.h"
#include <Core/CoreMgr.h>
#include <Resource/ResMgr.h>

//Specials
#include <Entities/Volume.h>
#include <Entities/StlMesh.h>
#include <Entities/Skybox.h>
#include <Entities/Flow3D.h>

namespace Engine
{
	class EntityReg
	{
	public:
		static void Register(EntityFactory *factory, CoreMgr *coreMgr)
		{
			//Register special C++ entity types
			factory->RegisterSpecial(Volume::GetStaticSpecialType(), &Volume::Create);
			factory->RegisterSpecial(StlMesh::GetStaticSpecialType(), &StlMesh::Create);
			factory->RegisterSpecial(Skybox::GetStaticSpecialType(), &Skybox::Create);
			factory->RegisterSpecial(Flow3D::GetStaticSpecialType(), &Flow3D::Create);

			//Register XML defined entities
			std::vector<CL_String> entities = coreMgr->getResMgr()->getFilesInDir("/XML/Entities/");
			for(unsigned int i = 0; i < entities.size(); i++)
			{
				factory->registerEntity(entities[i].c_str());
			}
		}
	};
}
