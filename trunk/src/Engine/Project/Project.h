#pragma once

namespace Engine
{
	class CoreMgr;

	class Project
	{
	public:
		Project(CoreMgr *coreMgr, const CL_String &name);
		~Project();

		const CL_String &getName() const { return name; }

	private:
		CoreMgr *coreMgr;
		CL_String name;
	};
}
