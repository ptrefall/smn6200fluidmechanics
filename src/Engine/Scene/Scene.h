#pragma once

namespace Engine
{
	class CoreMgr;
	class Scene
	{
	public:
		static void init_scene(CoreMgr *coreMgr, const CL_String &scene_script);
	};
}
