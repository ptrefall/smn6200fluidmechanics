#include "precomp.h"
#include "ExposeGuiEventParams.h"
#include "ExposeEvent.h"
#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>

#include "ScriptMgr.h"

using namespace Engine;
using namespace LuaPlus;

ExposeGuiEventParams::ExposeGuiEventParams(CoreMgr *coreMgr, ExposeEvent &e_event, Rocket::Core::Event& event)
: e_event(e_event), event(event)
{
	this->coreMgr = coreMgr;
	init();
}

ExposeGuiEventParams::~ExposeGuiEventParams()
{
}

void ExposeGuiEventParams::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	LuaObject &lEvent = e_event.getLEvent();
	lParams = lEvent.CreateTable("Parameters");

	const Rocket::Core::Dictionary *params = event.GetParameters();
	if(params == NULL || params->Size() <= 0)
		return;

	for(int i = 0; i < params->Size(); i++)
	{
		LuaObject lParam;
		Rocket::Core::String param_name;

		int pos = i;
		Rocket::Core::Variant *v;
		params->Iterate(pos, param_name, v);

		Rocket::Core::Variant::Type type = v->GetType();
		if(type == Rocket::Core::Variant::BYTE)
		{
			Rocket::Core::byte value = v->Get<Rocket::Core::byte>();
			lParam.AssignInteger(coreMgr->getScriptMgr()->GetGlobalState()->Get(), (int)value);
		}
		else if(type == Rocket::Core::Variant::CHAR)
		{
			char value = v->Get<char>();
			lParam.AssignInteger(coreMgr->getScriptMgr()->GetGlobalState()->Get(), (int)value);
		}
		else if(type == Rocket::Core::Variant::COLOURB)
		{
			Rocket::Core::Colourb value = v->Get<Rocket::Core::Colourb>();
			lParam.AssignNewTable(coreMgr->getScriptMgr()->GetGlobalState()->Get());
			lParam.SetInteger("red", (int)value.red);
			lParam.SetInteger("green", (int)value.green);
			lParam.SetInteger("blue", (int)value.blue);
			lParam.SetInteger("alpha", (int)value.alpha);
		}
		else if(type == Rocket::Core::Variant::COLOURF)
		{
			Rocket::Core::Colourf value = v->Get<Rocket::Core::Colourf>();
			lParam.AssignNewTable(coreMgr->getScriptMgr()->GetGlobalState()->Get());
			lParam.SetNumber("red", value.red);
			lParam.SetNumber("green", value.green);
			lParam.SetNumber("blue", value.blue);
			lParam.SetNumber("alpha", value.alpha);
		}
		else if(type == Rocket::Core::Variant::FLOAT)
		{
			float value = v->Get<float>();
			lParam.AssignNumber(coreMgr->getScriptMgr()->GetGlobalState()->Get(), value);
		}
		else if(type == Rocket::Core::Variant::INT)
		{
			int value = v->Get<int>();
			lParam.AssignInteger(coreMgr->getScriptMgr()->GetGlobalState()->Get(), value);
		}
		else if(type == Rocket::Core::Variant::STRING)
		{
			Rocket::Core::String value = v->Get<Rocket::Core::String>();
			lParam.AssignString(coreMgr->getScriptMgr()->GetGlobalState()->Get(), value.CString());
		}
		else if(type == Rocket::Core::Variant::VECTOR2)
		{
			Rocket::Core::Vector2f value = v->Get<Rocket::Core::Vector2f>();
			lParam.AssignNewTable(coreMgr->getScriptMgr()->GetGlobalState()->Get());
			lParam.SetNumber("x", value.x);
			lParam.SetNumber("y", value.y);
		}
		else if(type == Rocket::Core::Variant::WORD)
		{
			Rocket::Core::word value = v->Get<Rocket::Core::word>();
			lParam.AssignInteger(coreMgr->getScriptMgr()->GetGlobalState()->Get(), (int)value);
		}
		else continue;

		lParams.SetObject(param_name.CString(), lParam);
	}
}

LuaObject ExposeGuiEventParams::getLParam(const CL_String &name)
{
	return lParams.GetByName(name.c_str());
}
