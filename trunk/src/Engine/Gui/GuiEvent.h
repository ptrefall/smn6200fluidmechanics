#pragma once

namespace Engine
{
	class CoreMgr;

	class GuiEvent : public Rocket::Core::EventListener
	{
	public:
		GuiEvent(CoreMgr *coreMgr, const Rocket::Core::String& value);
		virtual ~GuiEvent();

		/// Sends the event value through to Invader's event processing system.
		virtual void ProcessEvent(Rocket::Core::Event& event);

		/// Destroys the event.
		virtual void OnDetach(Rocket::Core::Element* element);

	private:
		CoreMgr *coreMgr;
		Rocket::Core::String value;
	};
}
