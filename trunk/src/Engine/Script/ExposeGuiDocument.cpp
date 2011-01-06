#include "precomp.h"
#include "ExposeGuiDocument.h"
#include "ExposeGuiMgr.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <Event/Event.h>
#include <Event/EventValue.h>

using namespace Engine;
using namespace LuaPlus;

ExposeGuiDocument::ExposeGuiDocument(CoreMgr *coreMgr, ExposeGuiMgr *exposedGuiMgr, Rocket::Core::Element *document)
{
	this->coreMgr = coreMgr;
	this->exposedGuiMgr = exposedGuiMgr;
	this->document = document;
	init();
}

ExposeGuiDocument::~ExposeGuiDocument()
{
	lDocument.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
	lMeta.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeGuiDocument::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	CL_String name = document->GetId().CString();
	
	LuaObject &lDocuments = exposedGuiMgr->getLDocuments();
	lDocument = lDocuments.CreateTable(name.c_str());
	lDocument.SetString("tableType", "lDocument");
	lDocument.SetString("id", name.c_str());
	lDocument.SetString("name", name.c_str());
	lDocument.SetString("name_lc", CL_StringHelp::text_to_lower(name).c_str());
	lMeta = lDocument.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);

	//lMeta.RegisterDirect("SendEvent", *this, &ExposeGuiDocument::SendEvent);
	
	lDocument.SetLightUserData("__object", this);
	lDocument.SetMetaTable(lMeta);
}
