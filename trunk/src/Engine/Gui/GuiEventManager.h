#pragma once

namespace Engine
{
	class CoreMgr;

	class GuiEventManager
	{
	public:
		GuiEventManager(CoreMgr *coreMgr);
		~GuiEventManager();

		/// Processes an event coming through from Rocket.
		/// @param[in] event The Rocket event that spawned the application event.
		/// @param[in] value The application-specific event value.
		void ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value);
		
		/// Loads a window and binds the event handler for it.
		/// @param[in] window_name The name of the window to load.
		bool LoadWindow(const Rocket::Core::String& window_name);

	private:
		CoreMgr *coreMgr;
	};
}
