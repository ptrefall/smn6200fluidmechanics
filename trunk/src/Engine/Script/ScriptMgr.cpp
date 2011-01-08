#include "precomp.h"
#include "ScriptMgr.h"
#include "ExposeEntityManager.h"
#include "ExposeComponentManager.h"
#include "ExposeSceneMgr.h"
#include "ExposeGuiMgr.h"
#include "ExposeProjectMgr.h"
#include <Core/CoreMgr.h>
#include <Resource/ResMgr.h>

using namespace Engine;
using namespace LuaPlus;

ScriptMgr::ScriptMgr(CoreMgr *coreMgr)
: exposedEntityMgr(NULL), exposedComponentMgr(NULL), exposedSceneMgr(NULL), exposedGuiMgr(NULL), exposedProjectMgr(NULL)
{
	this->coreMgr = coreMgr;
}

ScriptMgr::~ScriptMgr()
{
	if(exposedEntityMgr)
	{
		delete exposedEntityMgr;
		exposedEntityMgr = NULL;
	}

	if(exposedComponentMgr)
	{
		delete exposedComponentMgr;
		exposedComponentMgr = NULL;
	}

	if(exposedSceneMgr)
	{
		delete exposedSceneMgr;
		exposedSceneMgr = NULL;
	}
	
	if(exposedGuiMgr)
	{
		delete exposedGuiMgr;
		exposedGuiMgr = NULL;
	}
	
	if(exposedProjectMgr)
	{
		delete exposedProjectMgr;
		exposedProjectMgr = NULL;
	}
}

void ScriptMgr::init()
{
	//Start LuaPlus here
	LuaObject globals = globalState->GetGlobals();
	globalState->OpenLibs();
	metaTable = globals.CreateTable("ScriptManagerMetaTable");
	metaTable.SetObject("__index", metaTable);

	LuaObject scriptManObj = globalState->BoxPointer(this);
	scriptManObj.SetMetaTable(metaTable);

	globalState->GetGlobals().SetObject("ScriptManager", scriptManObj);

	globals.RegisterDirect("Print", *this, &ScriptMgr::Print);

	exposedEntityMgr = new ExposeEntityManager(coreMgr);
	exposedComponentMgr = new ExposeComponentManager(coreMgr);
	exposedSceneMgr = new ExposeSceneMgr(coreMgr);
	exposedGuiMgr = new ExposeGuiMgr(coreMgr);
	exposedProjectMgr = new ExposeProjectMgr(coreMgr);
}

int ScriptMgr::doFile(const CL_String &fileName)
{
	try
	{
		int value = globalState->DoFile(cl_format("%1Scripts/%2", coreMgr->getResMgr()->getRootPath(), fileName).c_str());
		if(value > 0)
		{
			CL_String err = cl_format("There was a problem executing the file %1, \nerror: %2", fileName, LuaStackObject(globalState.Get(), -1).GetString());
			CL_Console::write_line(err);
		}
		return value;
	}
	catch(const LuaPlus::LuaException &e)
	{
		CL_String err = cl_format("An error occured when trying to extract a file: %1 \n%2", e.GetErrorMessage(), LuaStackObject(globalState.Get(), -1).GetString());
		CL_Console::write_line(err);
		return 1;
	}
}

int ScriptMgr::doString(const CL_String &script, bool throwException, bool logError)
{
	if(throwException)
	{
		int value = globalState->DoString(script.c_str());
		return value;
	}
	else
	{
		try
		{
			int value = globalState->DoString(script.c_str());
			if(value > 0)
			{
				if(logError)
				{
					CL_String err = cl_format("There was a problem executing the string %1, \nerror: %2", script, LuaStackObject(globalState.Get(), -1).GetString());
					CL_Console::write_line(err);
				}
			}
			return value;
		}
		catch(const LuaPlus::LuaException &e)
		{
			if(logError)
			{
				CL_String err = cl_format("An error occured when trying to extract a script: %1 \n%2", e.GetErrorMessage(), LuaStackObject(globalState.Get(), -1).GetString());
				CL_Console::write_line(err);
			}
			return 1;
		}
	}
}

CL_String ScriptMgr::getErrorOnStack()
{
	return CL_String(LuaStackObject(globalState.Get(), -1).GetString());
}

void ScriptMgr::Print(LuaObject text)
{
	if(!text.IsString() && !text.IsBoolean() && !text.IsNumber())
	{
		CL_String type_name = text.TypeName();
		CL_String err = cl_format("%1%2", "ScriptError: The type of text is", type_name);
		CL_Console::write_line(err);
		return;
	}
	
	if(text.IsString())
	{
		CL_String value = text.ToString();
		CL_Console::write_line(value);
	}
	else if(text.IsBoolean())
	{
		bool value = text.GetBoolean();
		if(value)
			CL_Console::write_line("true");
		else
			CL_Console::write_line("false");
	}
	else if(text.IsNumber())
	{
		float value = text.GetFloat();
		CL_Console::write_line(cl_format("%1", value));
	}
}
