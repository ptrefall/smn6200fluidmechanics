#pragma once

namespace Engine
{
	class CoreMgr;
	class IEntity;

	class Project;
	class GuiProjectTable;
	class GuiProjectTableEditFormatter;

	class ProjectMgr
	{
	public:
		ProjectMgr(CoreMgr *coreMgr);
		~ProjectMgr();

		bool createProject(const CL_String &filename);
		bool loadProject(const CL_String &filename);
		bool addEntity(IEntity *entity);
		void selectEntity(IEntity *entity);
		IEntity *getSelectedEntity();

		void save();
		Project *getProject() const { return project; }

	private:
		CoreMgr *coreMgr;

		Project *project;
		GuiProjectTable *table;
		GuiProjectTableEditFormatter *table_edit_formatter;
	};
}
