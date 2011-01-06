#include "precomp.h"
#include "ExposeGuiContext.h"
#include "ExposeGuiMgr.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <Event/Event.h>
#include <Event/EventValue.h>

using namespace Engine;
using namespace LuaPlus;

ExposeGuiContext::ExposeGuiContext(CoreMgr *coreMgr, ExposeGuiMgr *exposedGuiMgr, Rocket::Core::Context *context)
{
	this->coreMgr = coreMgr;
	this->exposedGuiMgr = exposedGuiMgr;
	this->context = context;
	init();
}

ExposeGuiContext::~ExposeGuiContext()
{
	lContext.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
	lMeta.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeGuiContext::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	CL_String name = context->GetName().CString();
	
	LuaObject &lContexts = exposedGuiMgr->getLContexts();
	lContext = lContexts.CreateTable(name.c_str());
	lContext.SetString("tableType", "lContext");
	lContext.SetString("id", name.c_str());
	lContext.SetString("name", name.c_str());
	lContext.SetString("name_lc", CL_StringHelp::text_to_lower(name).c_str());
	lMeta = lContext.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);

	//lMeta.RegisterDirect("SendEvent", *this, &ExposeGuiContext::SendEvent);
	
	lContext.SetLightUserData("__object", this);
	lContext.SetMetaTable(lMeta);
}
