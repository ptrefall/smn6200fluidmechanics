#pragma once

namespace Engine
{
	class CoreMgr;

	class GuiEventInstancer : public Rocket::Core::EventListenerInstancer
	{		
	public:
		GuiEventInstancer(CoreMgr *coreMgr);
		~GuiEventInstancer();

		// Instance an event listener object.
		// @param value Value of the event.
		virtual Rocket::Core::EventListener* InstanceEventListener(const Rocket::Core::String& value);

		// Releases this event listener instancer.
		virtual void Release();

	private:
		CoreMgr* coreMgr;
	};
}
