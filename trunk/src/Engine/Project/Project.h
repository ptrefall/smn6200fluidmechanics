#pragma once

namespace Engine
{
	class CoreMgr;
	class IEntity;

	class Project
	{
	public:
		Project(CoreMgr *coreMgr, const CL_String &name);
		~Project();

		const CL_String &getName() const { return name; }
		bool addEntity(IEntity *entity);
		void selectEntity(IEntity *entity);
		IEntity *getSelectedEntity() const { return selectedEntity; }

		void PlayProjectAnimation();
		void StopProjectAnimation();
		void RewindProjectAnimation();
		void ForwardProjectAnimation();

		void save();

	private:
		CoreMgr *coreMgr;
		CL_String name;

		IEntity *selectedEntity;

		std::vector<IEntity*> stl_meshes;
		std::vector<IEntity*> unsaved_stl_meshes;

		std::vector<IEntity*> flw_sims;
		std::vector<IEntity*> unsaved_flw_sims;

		std::vector<IEntity*> misc_entities;
		std::vector<IEntity*> unsaved_misc_entities;
	};
}
