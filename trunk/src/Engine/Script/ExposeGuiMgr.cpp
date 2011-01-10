#include "precomp.h"
#include "ExposeGuiMgr.h"
#include "ExposeGuiContext.h"
#include "ExposeGuiDocument.h"
#include <Core/CoreMgr.h>
#include <Gui/GuiMgr.h>
#include <Resource/ResMgr.h>
#include <Core/Cam.h>

#include <Event/IEventManager.h>
#include "ScriptMgr.h"

using namespace Engine;
using namespace LuaPlus;

ExposeGuiMgr::ExposeGuiMgr(CoreMgr *coreMgr)
: engineEvents(coreMgr->getEventMgr())
{
	this->coreMgr = coreMgr;
	init();
}

ExposeGuiMgr::~ExposeGuiMgr()
{
}

void ExposeGuiMgr::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	globals.RegisterDirect("CreateGuiContext", *this, &ExposeGuiMgr::CreateGuiContext);
	globals.RegisterDirect("CreateGuiDocument", *this, &ExposeGuiMgr::CreateGuiDocument);
	globals.RegisterDirect("ZoomInCamera", *this, &ExposeGuiMgr::ZoomInCamera);
	globals.RegisterDirect("ZoomOutCamera", *this, &ExposeGuiMgr::ZoomOutCamera);
	globals.RegisterDirect("MoveUpCamera", *this, &ExposeGuiMgr::MoveUpCamera);
	globals.RegisterDirect("MoveDownCamera", *this, &ExposeGuiMgr::MoveDownCamera);
	globals.RegisterDirect("PitchCamera", *this, &ExposeGuiMgr::PitchCamera);
	globals.RegisterDirect("YawCamera", *this, &ExposeGuiMgr::YawCamera);
	lContexts = globals.CreateTable("GuiContexts");
	lDocuments = globals.CreateTable("GuiDocuments");

	engineEvents.Connect("GuiContextCreated", this, &ExposeGuiMgr::OnContextCreated);
	engineEvents.Connect("GuiContextRemoved", this, &ExposeGuiMgr::OnContextRemoved);
	engineEvents.Connect("GuiDocumentCreated", this, &ExposeGuiMgr::OnDocumentCreated);
	engineEvents.Connect("GuiDocumentRemoved", this, &ExposeGuiMgr::OnDocumentRemoved);

	//Load all scripts in Scripts/Gui
	std::vector<CL_String> scripts = coreMgr->getResMgr()->getFilesInDir("/Scripts/Gui/");
	for(unsigned int i = 0; i < scripts.size(); i++)
	{
		int fail = coreMgr->getScriptMgr()->doFile(cl_format("Gui/%1", scripts[i]));
		if(fail)
		{
			CL_String err = cl_format("Failed to load gui script %1", scripts[i]);
			throw CL_Exception(err);
		}
	}
}

LuaObject ExposeGuiMgr::getLContext(const CL_String &name)
{
	return lContexts.GetByName(name.c_str());
}

LuaObject ExposeGuiMgr::getLDocument(const CL_String &name)
{
	return lDocuments.GetByName(name.c_str());
}

ExposeGuiContext *ExposeGuiMgr::getExposedGuiContext(const CL_String &name)
{
	for(unsigned int i = 0; i < exposedContexts.size(); i++)
	{
		if(exposedContexts[i]->getContext()->GetName() == Rocket::Core::String(name.c_str()))
		{
			return exposedContexts[i];
		}
	}
	return NULL;
}

ExposeGuiDocument *ExposeGuiMgr::getExposedGuiDocument(const CL_String &name)
{
	for(unsigned int i = 0; i < exposedDocuments.size(); i++)
	{
		if(exposedDocuments[i]->getDocument()->GetId() == Rocket::Core::String(name.c_str()))
		{
			return exposedDocuments[i];
		}
	}
	return NULL;
}

LuaObject ExposeGuiMgr::CreateGuiContext(LuaPlus::LuaObject lName, LuaPlus::LuaObject lWidth, LuaPlus::LuaObject lHeight)
{
	if(!lName.IsString())
	{
		CL_String msg = cl_format("Failed to create context, because the type of name was %1 when expecting String", lName.TypeName());
		throw CL_Exception(msg);
	}
	if(!lWidth.IsNumber() && !lWidth.IsInteger())
	{
		CL_String msg = cl_format("Failed to create context, because the type of width was %1 when expecting Number or Integer", lWidth.TypeName());
		throw CL_Exception(msg);
	}
	if(!lHeight.IsNumber() && !lHeight.IsInteger())
	{
		CL_String msg = cl_format("Failed to create context, because the type of height was %1 when expecting Number or Integer", lHeight.TypeName());
		throw CL_Exception(msg);
	}

	CL_String name = lName.ToString();
	int width = 0;
	int height = 0;
	
	if(lWidth.IsNumber())
		width = (int)lWidth.GetFloat();
	else
		width = lWidth.ToInteger();

	if(lHeight.IsNumber())
		height = (int)lHeight.GetFloat();
	else
		height = lHeight.ToInteger();
	
	Rocket::Core::Context *context = coreMgr->getGuiMgr()->addContext(name, width, height);
	ExposeGuiContext *exposedContext = getExposedGuiContext(context->GetName().CString());

	if(exposedContext)
		return exposedContext->getLContext();
	else
		return LuaObject(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

LuaObject ExposeGuiMgr::CreateGuiDocument(LuaPlus::LuaObject lContextName, LuaPlus::LuaObject lPath)
{
	if(!lContextName.IsString())
	{
		CL_String msg = cl_format("Failed to create context, because the type of context name was %1 when expecting String", lContextName.TypeName());
		throw CL_Exception(msg);
	}

	if(!lPath.IsString())
	{
		CL_String msg = cl_format("Failed to create context, because the type of path was %1 when expecting String", lPath.TypeName());
		throw CL_Exception(msg);
	}
	

	CL_String name = lContextName.ToString();
	CL_String path = lPath.ToString();
	
	Rocket::Core::ElementDocument *document = coreMgr->getGuiMgr()->addDocument(name, cl_format("%1Gui/%2.rml", coreMgr->getResMgr()->getRootPath(), path));
	ExposeGuiDocument *exposedDocument = getExposedGuiDocument(document->GetId().CString());

	if(exposedDocument)
		return exposedDocument->getLDocument();
	else
		return LuaObject(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeGuiMgr::ZoomInCamera(LuaObject lValue)
{
	if(!lValue.IsNumber())
	{
		CL_String msg = cl_format("Failed to zoom camera, because the type of value was %1 when expecting number", lValue.TypeName());
		throw CL_Exception(msg);
	}

	coreMgr->getCam()->moveForward(lValue.GetFloat());
}

void ExposeGuiMgr::ZoomOutCamera(LuaObject lValue)
{
	if(!lValue.IsNumber())
	{
		CL_String msg = cl_format("Failed to zoom camera, because the type of value was %1 when expecting number", lValue.TypeName());
		throw CL_Exception(msg);
	}

	coreMgr->getCam()->moveBackward(lValue.GetFloat());
}

void ExposeGuiMgr::MoveUpCamera(LuaObject lValue)
{
	if(!lValue.IsNumber())
	{
		CL_String msg = cl_format("Failed to move camera, because the type of value was %1 when expecting number", lValue.TypeName());
		throw CL_Exception(msg);
	}

	coreMgr->getCam()->moveUp(lValue.GetFloat());
}

void ExposeGuiMgr::MoveDownCamera(LuaObject lValue)
{
	if(!lValue.IsNumber())
	{
		CL_String msg = cl_format("Failed to move camera, because the type of value was %1 when expecting number", lValue.TypeName());
		throw CL_Exception(msg);
	}

	coreMgr->getCam()->moveDown(lValue.GetFloat());
}

void ExposeGuiMgr::PitchCamera(LuaObject lValue)
{
	if(!lValue.IsNumber())
	{
		CL_String msg = cl_format("Failed to pitch camera, because the type of value was %1 when expecting number", lValue.TypeName());
		throw CL_Exception(msg);
	}

	float val = lValue.GetFloat();
	if(val > 0.0f)
	{
		coreMgr->getCam()->pitchUp(val);
	}
	else
	{
		coreMgr->getCam()->pitchDown(-1.0f*val);
	}
}

void ExposeGuiMgr::YawCamera(LuaObject lValue)
{
	if(!lValue.IsNumber())
	{
		CL_String msg = cl_format("Failed to yaw camera, because the type of value was %1 when expecting number", lValue.TypeName());
		throw CL_Exception(msg);
	}

	float val = lValue.GetFloat();
	if(val > 0.0f)
	{
		coreMgr->getCam()->yawRight(val);
	}
	else
	{
		coreMgr->getCam()->yawLeft(-1.0f*val);
	}
}

void ExposeGuiMgr::OnContextCreated(const Events::Event &event)
{
	Rocket::Core::Context *context = event.getArgument(0).ToGuiContext();

	ExposeGuiContext *exposedContext = new ExposeGuiContext(coreMgr, this, context);
	exposedContexts.push_back(exposedContext);
}

void ExposeGuiMgr::OnContextRemoved(const Events::Event &event)
{
	Rocket::Core::Context *context = event.getArgument(0).ToGuiContext();
	for(unsigned int i = 0; i < exposedContexts.size(); i++)
	{
		if(exposedContexts[i]->getContext()->GetName() == context->GetName())
		{
			ExposeGuiContext *exposedContext = exposedContexts[i];
			exposedContexts.erase(exposedContexts.begin() + i);
			delete exposedContext;
			exposedContext = NULL;
			break;
		}
	}
}

void ExposeGuiMgr::OnDocumentCreated(const Events::Event &event)
{
	Rocket::Core::Element *document = event.getArgument(0).ToGuiDocument();

	ExposeGuiDocument *exposedDocument = new ExposeGuiDocument(coreMgr, this, document);
	exposedDocuments.push_back(exposedDocument);
}

void ExposeGuiMgr::OnDocumentRemoved(const Events::Event &event)
{
	Rocket::Core::Element *document = event.getArgument(0).ToGuiDocument();
	for(unsigned int i = 0; i < exposedDocuments.size(); i++)
	{
		if(exposedDocuments[i]->getDocument()->GetId() == document->GetId())
		{
			ExposeGuiDocument *exposedDocument = exposedDocuments[i];
			exposedDocuments.erase(exposedDocuments.begin() + i);
			delete exposedDocument;
			exposedDocument = NULL;
			break;
		}
	}
}
