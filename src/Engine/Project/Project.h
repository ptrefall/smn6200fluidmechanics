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

		void save();

	private:
		CoreMgr *coreMgr;
		CL_String name;

		std::vector<IEntity*> entities;
		std::vector<IEntity*> unsaved_entities;
	};
}
