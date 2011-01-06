#pragma once

#include <LuaPlus/LuaPlus.h>

namespace Engine
{
class CoreMgr;

class ExposeGuiMgr;

class ExposeGuiDocument
{
public:
	ExposeGuiDocument(CoreMgr *coreMgr, ExposeGuiMgr *exopsedGuiMgr, Rocket::Core::Element *document);
	~ExposeGuiDocument();

	Rocket::Core::Element *getDocument() const { return document; }
	LuaPlus::LuaObject getLDocument() const { return lDocument; }
	LuaPlus::LuaObject getLMeta() const { return lMeta; }

protected:
	void init();

	CoreMgr *coreMgr;
	ExposeGuiMgr *exposedGuiMgr;

	Rocket::Core::Element *document;
	LuaPlus::LuaObject lDocument;
	LuaPlus::LuaObject lMeta;
};

}
