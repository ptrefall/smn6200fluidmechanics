#include "precomp.h"
#include "GuiEvent.h"
#include "GuiEventManager.h"
#include <Core/CoreMgr.h>
#include "GuiMgr.h"

using namespace Engine;

GuiEvent::GuiEvent(CoreMgr *coreMgr, const Rocket::Core::String& value)
: coreMgr(coreMgr), value(value)
{
}

GuiEvent::~GuiEvent()
{
}

// Sends the event value through to Engine's event processing system.
void GuiEvent::ProcessEvent(Rocket::Core::Event& event)
{
	coreMgr->getGuiMgr()->getGuiEventMgr()->ProcessEvent(event, value);
}

// Destroys the event.
void GuiEvent::OnDetach(Rocket::Core::Element* ROCKET_UNUSED(element))
{
	delete this;
}