#pragma once

namespace Engine
{
	class CoreMgr;
	class Project;

	class ProjectMgr
	{
	public:
		ProjectMgr(CoreMgr *coreMgr);
		~ProjectMgr();

		bool createProject(const CL_String &filename);
		bool loadProject(const CL_String &filename);

		Project *getProject() const { return project; }

	private:
		CoreMgr *coreMgr;

		Project *project;
	};
}
