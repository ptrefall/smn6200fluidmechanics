#include "precomp.h"
#include "GuiEventManager.h"
#include <Core/CoreMgr.h>
#include "GuiMgr.h"
#include <Resource/ResMgr.h>
#include <Script/ScriptMgr.h>
#include <Script/ExposeEvent.h>
#include <Event/Event.h>
#include <Event/EventValue.h>

using namespace Engine;

GuiEventManager::GuiEventManager(CoreMgr *coreMgr)
: coreMgr(coreMgr)
{
}

GuiEventManager::~GuiEventManager()
{
}

// Processes an event coming through from Rocket.
void GuiEventManager::ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
{
	Rocket::Core::StringList commands;
	Rocket::Core::StringUtilities::ExpandString(commands, value, ';');
	for (size_t i = 0; i < commands.size(); ++i)
	{
		// Check for a generic 'load' or 'exit' command.
		Rocket::Core::StringList values;
		Rocket::Core::StringUtilities::ExpandString(values, commands[i], ' ');

		if (values.empty())
			return;

		if (values[0] == "goto" &&
 			values.size() > 1)
		{
			// Load the window, and if successful close the old window.
			if (LoadWindow(values[1]))
				event.GetTargetElement()->GetOwnerDocument()->Close();
		}
		else if (values[0] == "load" &&
 			values.size() > 1)
		{
			// Load the window.
			LoadWindow(values[1]);
		}
		else if (values[0] == "close")
		{
			Rocket::Core::ElementDocument* target_document = NULL;

			if (values.size() > 1)
				target_document = coreMgr->getGuiMgr()->getContext(0)->GetDocument(values[1].CString());
			else
				target_document = event.GetTargetElement()->GetOwnerDocument();

			if (target_document != NULL)
				target_document->Close();
		}
		else if (values[0] == "exit")
		{
			coreMgr->exit();
		}
		else
		{
			//Ok, so this C++ side didn't handle any of the simple hardcoded commands, maybe it's sending a script?
			Events::EventValue arg1(event.GetTargetElement());
			Events::EventValue arg2(coreMgr->getGuiMgr()->getContext(0));
			Events::Event evt(event.GetType().CString(), /*arg0,*/ arg1, arg2);
			ExposeEvent exposedEvent(coreMgr, &evt);
			//ExposeGuiEventParams exposedParams(coreMgr, exposedEvent);
			//	//(*event.GetParameters())[0]->
			coreMgr->getScriptMgr()->doString(cl_format("%1(Events['%2'])", commands[i].CString(), event.GetType().CString()));
			
		}
	}
}

// Loads a window and binds the event handler for it.
bool GuiEventManager::LoadWindow(const Rocket::Core::String& window_name)
{
	coreMgr->getGuiMgr()->addDocument("Main", cl_format("%1Gui/%2.rml", coreMgr->getResMgr()->getRootPath(), window_name.CString()));
	return true;
}
