#include "precomp.h"
#include "ExposeIEntity.h"
#include "ExposeEntityManager.h"
#include "ExposePropertyContainer.h"
#include "ExposeComponentContainer.h"
#include "ExposeCurve.h"
#include "ExposeBezier.h"
#include "ExposeERBS.h"
#include "ScriptMgr.h"
#include <Core/CoreMgr.h>
#include <Entity/IEntity.h>
#include <Entities/Curve.h>
#include <Entities/Bezier.h>
#include <Entities/ERBS.h>
#include <Event/Event.h>
#include <Event/EventValue.h>

using namespace Engine;
using namespace LuaPlus;

ExposeIEntity::ExposeIEntity(CoreMgr *coreMgr, ExposeEntityManager *exposedEntityMgr, IEntity *entity)
: exposedPropContainer(NULL), exposedCompContainer(NULL), exposedCurve(NULL), exposedBezier(NULL), exposedERBS(NULL)
{
	this->coreMgr = coreMgr;
	this->exposedEntityMgr = exposedEntityMgr;
	this->entity = entity;
	init();
}

ExposeIEntity::~ExposeIEntity()
{
	if(exposedPropContainer)
	{
		delete exposedPropContainer;
		exposedPropContainer = NULL;
	}
	if(exposedCompContainer)
	{
		delete exposedCompContainer;
		exposedCompContainer = NULL;
	}
	if(exposedCurve)
	{
		delete exposedCurve;
		exposedCurve = NULL;
	}

	lEntity.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
	lMeta.AssignNil(coreMgr->getScriptMgr()->GetGlobalState()->Get());
}

void ExposeIEntity::init()
{
	LuaObject globals = (*coreMgr->getScriptMgr()->GetGlobalState())->GetGlobals();
	unsigned int entityId = entity->getId();
	
	LuaObject &lEntities = exposedEntityMgr->getLEntities();
	lEntity = lEntities.CreateTable((int)entityId);
	lEntity.SetString("tableType", "IEntity");
	lEntity.SetInteger("id", (int)entityId);
	lEntity.SetString("type", entity->getType().c_str());
	lEntity.SetString("name", entity->getName().c_str());
	lEntity.SetString("name_lc", CL_StringHelp::text_to_lower(entity->getName()).c_str());
	lMeta = lEntity.CreateTable("MetaTable");
	lMeta.SetObject("__index", lMeta);

	lMeta.RegisterDirect("SendEvent", *this, &ExposeIEntity::SendEvent);
	lMeta.RegisterDirect("SendCommand", *this, &ExposeIEntity::SendCommand);
	lMeta.RegisterDirect("HasComponent", *this, &ExposeIEntity::HasComponent);
	
	lEntity.SetLightUserData("__object", this);
	lEntity.SetMetaTable(lMeta);

	exposedPropContainer = new ExposePropertyContainer(coreMgr, this);
	exposedCompContainer = new ExposeComponentContainer(coreMgr, this);

	if(entity->getSpecialType() == Curve::GetStaticSpecialType())
	{
		exposedCurve = new ExposeCurve(coreMgr, lEntity, lMeta, dynamic_cast<Curve*>(entity));
	}
	else if(entity->getSpecialType() == Bezier::GetStaticSpecialType())
	{
		exposedBezier = new ExposeBezier(coreMgr, lEntity, lMeta, dynamic_cast<Bezier*>(entity));
	}
	else if(entity->getSpecialType() == ERBS::GetStaticSpecialType())
	{
		exposedERBS = new ExposeERBS(coreMgr, lEntity, lMeta, dynamic_cast<ERBS*>(entity));
	}
}

void ExposeIEntity::SendCommand(LuaObject lSelf, LuaObject lCommand)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		throw CL_Exception(msg);
	}

	if(!lCommand.IsString())
	{
		CL_String msg = cl_format("CommandType was not a string (it's a %1)", lCommand.TypeName());
		throw CL_Exception(msg);
	}

	entity->ExecuteCommandOnComponents(lCommand.ToString());
}

void ExposeIEntity::SendEvent(LuaObject lSelf, LuaObject lEventType, LuaObject lEventArg)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		throw CL_Exception(msg);
	}

	if(!lEventType.IsString())
	{
		CL_String msg = cl_format("EventType was not a string (it's a %1)", lEventType.TypeName());
		throw CL_Exception(msg);
	}

	if(!lEventArg.IsString() && !lEventArg.IsBoolean() && !lEventArg.IsNumber() && !lEventArg.IsTable() && !lEventArg.IsNil())
	{
		CL_String msg = cl_format("EventArg was not a string, bool, number, table or nil (it's a %1)", lEventArg.TypeName());
		throw CL_Exception(msg);
	}

	CL_String eventType = lEventType.ToString();
	if(lEventArg.IsString())
	{
		CL_String arg = lEventArg.ToString();
		entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
	}
	else if(lEventArg.IsBoolean())
	{
		bool arg = lEventArg.GetBoolean();
		entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
	}
	else if(lEventArg.IsNumber())
	{
		float arg = lEventArg.GetFloat();
		entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
	}
	else if(lEventArg.IsTable())
	{	
		LuaObject xObj = lEventArg.GetByName("x");

		//If tableType is nil, then this should be a vector
		if(xObj.IsNil())
			throw CL_Exception("Tried to send event, and though argument was a table, it was not a CL_Vec, as expected!");

		LuaObject yObj = lEventArg.GetByName("y");
		LuaObject zObj = lEventArg.GetByName("z");
		LuaObject wObj = lEventArg.GetByName("w");

		int vecCounter = 1; //account for x
		if(!yObj.IsNil())
			vecCounter++;
		if(!zObj.IsNil())
			vecCounter++;
		if(!wObj.IsNil())
			vecCounter++;
		
		if(vecCounter == 1)
		{
			float arg = xObj.GetFloat();
			entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
		}
		else if(vecCounter == 2)
		{
			CL_Vec2f arg = CL_Vec2f(xObj.GetFloat(), yObj.GetFloat());
			entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
		}
		else if(vecCounter == 3)
		{
			CL_Vec3f arg = CL_Vec3f(xObj.GetFloat(), yObj.GetFloat(), zObj.GetFloat());
			entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
		}
		else if(vecCounter == 4)
		{
			CL_Vec4f arg = CL_Vec4f(xObj.GetFloat(), yObj.GetFloat(), zObj.GetFloat(), wObj.GetFloat());
			entity->ExecuteEventOnComponents(Events::Event(eventType, Events::EventValue(arg)));
		}
		else
			throw CL_Exception("Tried to send event, but arg didn't match any known CL_Vec type!");
	}
}

LuaPlus::LuaObject ExposeIEntity::HasComponent(LuaPlus::LuaObject lSelf, LuaPlus::LuaObject lName)
{
	if(!lSelf.IsTable())
	{
		CL_String msg = cl_format("Self was not a table (it's a %1)", lSelf.TypeName());
		throw CL_Exception(msg);
	}

	if(!lName.IsString())
	{
		CL_String msg = cl_format("Name was not a string (it's a %1)", lName.TypeName());
		throw CL_Exception(msg);
	}

	LuaObject lHasComp;
	lHasComp.AssignBoolean(coreMgr->getScriptMgr()->GetGlobalState()->Get(), entity->HasComponent(lName.ToString()));
	return lHasComp;
}
