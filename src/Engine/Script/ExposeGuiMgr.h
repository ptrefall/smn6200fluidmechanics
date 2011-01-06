#pragma once

#include <LuaPlus/LuaPlus.h>
#include <Event/EventContainer.h>
#include <Event/Event.h>

namespace Engine
{
class CoreMgr;
class ExposeGuiContext;
class ExposeGuiDocument;

class ExposeGuiMgr
{
public:
	ExposeGuiMgr(CoreMgr *coreMgr);
	~ExposeGuiMgr();

	LuaPlus::LuaObject &getLContexts() { return lContexts; }
	LuaPlus::LuaObject getLContext(const CL_String &name);
	ExposeGuiContext *getExposedGuiContext(const CL_String &name);

	LuaPlus::LuaObject &getLDocuments() { return lDocuments; }
	LuaPlus::LuaObject getLDocument(const CL_String &name);
	ExposeGuiDocument *getExposedGuiDocument(const CL_String &name);

private:
	void init();

	LuaPlus::LuaObject CreateGuiContext(LuaPlus::LuaObject lName, LuaPlus::LuaObject lWidth, LuaPlus::LuaObject lHeight);
	LuaPlus::LuaObject CreateGuiDocument(LuaPlus::LuaObject lContextName, LuaPlus::LuaObject lPath);

	CoreMgr *coreMgr;

	LuaPlus::LuaObject lContexts;
	std::vector<ExposeGuiContext*> exposedContexts;

	LuaPlus::LuaObject lDocuments;
	std::vector<ExposeGuiDocument*> exposedDocuments;

	void OnContextCreated(const Events::Event &event);
	void OnContextRemoved(const Events::Event &event);
	void OnDocumentCreated(const Events::Event &event);
	void OnDocumentRemoved(const Events::Event &event);
	Events::EventContainer engineEvents;
};

}
