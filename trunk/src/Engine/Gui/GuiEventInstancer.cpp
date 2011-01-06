#include "precomp.h"
#include "GuiEventInstancer.h"
#include "GuiEvent.h"

using namespace Engine;

GuiEventInstancer::GuiEventInstancer(CoreMgr *coreMgr)
: coreMgr(coreMgr)
{
}

GuiEventInstancer::~GuiEventInstancer()
{
}

// Instances a new event handle for Invaders.
Rocket::Core::EventListener* GuiEventInstancer::InstanceEventListener(const Rocket::Core::String& value)
{
	return new GuiEvent(coreMgr, value);
}

// Destroys the instancer.
void GuiEventInstancer::Release()
{
	delete this;
}