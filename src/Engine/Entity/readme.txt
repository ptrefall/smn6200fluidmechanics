COMPONENT-BASED ENTITY SYSTEM
------------------------------
by Pål Solberg Trefall and Kenneth Gangstø




LICENSE
------------------------
Component-based Entity Engine
Copyright (c) 2009 Pål Trefall and Kenneth Gangstø

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Note: Some of the libraries Component-based Entity Engine may link to may have additional
requirements or restrictions.




DEPENDENCIES
--------------
ClanLib 2.0 (http://www.clanlib.org)




HOWTO/FAQ
----------
1)What is a game object?
- A Game Engine side object that inherits from Entity in Entity engine, 
to wrap the Entity engine functionality, and add additional Game Engine 
specific functionality to the Game Engine's Game Objects.

2)What is an Entity?
- A wrapper of ComponentContainer and PropertyContainer.

3)What is a ComponentContainer?
- Holds a map of components, and robust/secure methods for adding and manipulating components.

4)What is a PropertyContainer?
- Holds a map of properties, and robust/secure methods for adding/removing and manipulating properties.

5)What is a Property?
- Entity Engine's secure variable for data storage, including a smart pointer. It's owned by it's
ComponentContainer/Entity. It serves as glue between components, as components can listen for an altered
property and act upon such changes. Two components can work on the same property (as Entity is the owner).
This allows multiple components to work on the Position property of an Entity for instance, without them
ever knowing about each other.

6)What is a Component?
- Entity Engine's modular chuncks of logic for entities. Instead of using inheritance to define new entity
types, a modular approach is used. This seperates logic cleanly, makes extending objects simple and the
definition of a new game object data driven. A new game object definition, is simply about defining which
components it holds.

7)How to register new components with the entity engine?
- componentFactory().RegisterComponent(ExamineComponent::GetType(), &ExamineComponent::Create);
- componentFactory().RegisterComponent(MovementComponent::GetType(), &MovementComponent::Create);

8)How to attach a component to a game object?
- Some initialization method for a zone:

GameObject *objectRobot = new GameObject(componentFactory, zone);
objectRobot->AddComponent("Examine");
objectRobot->AddComponent("Movement");
objectRobot->GetProperty<CL_String>("Description") = "It's a useless robot!";
objectRobot->GetProperty<float>("MovementSpeed") = 50.0f;

9)How would you define a game object in XML?

<object type="Robot">
  <components>
    <component>Examine</component>
    <component>Movement</component>
  </components>
  <properties>
    <property name="Description">It's a useless robot!</property>
    <property name="MovementSpeed">50.0</property>
  </properties>
</object>


10)How to write a new component from scratch
- ExamineComponent.h
---------------------------

#pragma once

#include <Entity/Component.h>
#include <Entity/Property.h>

namespace Entity { class IEntity; }

class ServerPlayer;

class ExamineComponent : public Entity::Component
{
public:
	ExamineComponent(Entity::IEntity* entity, const CL_String &name);
	virtual ~ExamineComponent() {}

	virtual void RequestCommands(std::vector<CL_String> &requestCommands, ServerPlayer *player);
	virtual void ExecuteCommand(const CL_String &command, ServerPlayer *player);

	static CL_String GetType() { return "Examine"; }
	static Entity::Component* Create(Entity::IEntity* entity, const CL_String &name) { return new ExamineComponent(entity, name); }

protected:
	Entity::Property<CL_String> description;

	void OnExamine(ServerPlayer *player);
};
----------------------------
- ExamineComponent.cpp
----------------------------

#include "precomp.h"
#include "ExamineComponent.h"
#include "../GameObject.h"
#include "../ServerPlayer.h"
#include <Entity/Entity.h>
#include <NetworkSharedLibrary/NetEvents.h>

using namespace Entity;

ExamineComponent::ExamineComponent(IEntity* entity, const CL_String &name)
: Component(entity, name)
{
	description = entity->AddProperty<CL_String>("Description", CL_String());
}

void ExamineComponent::RequestCommands(std::vector<CL_String> &requestCommands, ServerPlayer *player)
{
	requestCommands.push_back("Examine");
}

void ExamineComponent::ExecuteCommand(const CL_String &command, ServerPlayer *player)
{
	if(command == "Examine")
	{
		OnExamine(player);
	}
}

void ExamineComponent::OnExamine(ServerPlayer *player)
{
	GameObject *gameObject = (GameObject *)entity;
	gameObject->SendViewEvent(CL_NetGameEvent(STC_OBJECT_DESCRIPTION, description.Get()), player->GetConnection());
}
------------------------------------
- MovementComponent.h
------------------------------------
#pragma once

#include <Entity/Component.h>
#include <Entity/Property.h>

namespace Entity { class IEntity; }

class MovementComponent : public Entity::Component
{
public:
	MovementComponent(Entity::IEntity* entity, const CL_String &name);
	virtual ~MovementComponent() {}

	virtual void Update(int deltaTime);

	static CL_String GetType() { return "Movement"; }
	static Entity::Component* Create(Entity::IEntity* entity, const CL_String &name) { return new MovementComponent(entity, name); }

protected:
	Entity::Property<float> movementSpeed;
	Entity::Property<CL_Pointf> newDestinationPosition;
	std::vector<CL_Pointf> movementDestinations;

	CL_Slot slotNewDestinationPositionChanged;
	void OnNewDestinationPosition(const CL_Pointf &oldValue, const CL_Pointf &newValue);

	void ClearDestinationPositions();
	void AddDestinationPosition(const CL_Pointf &position);
	void SendDestinationPositions();
};
----------------------------------------
- MovementComponent.cpp
----------------------------------------
#include "precomp.h"
#include "MovementComponent.h"
#include "../GameObject.h"
//...
#include <Entity/Entity.h>
#include <NetworkSharedLibrary/NetEvents.h>

using namespace Entity;

MovementComponent::MovementComponent(IEntity* entity, const CL_String &name)
: Component(entity, name)
{
	movementSpeed = entity->AddProperty<float>("MovementSpeed", 100.0f);
	newDestinationPosition = entity->AddProperty<CL_Pointf>("NewDestinationPosition", CL_Pointf());
	slotNewDestinationPositionChanged = newDestinationPosition.ValueChanged().connect(this, &MovementComponent::OnNewDestinationPosition);

	GameObject *gameObject = (GameObject*)entity;
	gameObject->AddClientComponentRequirement(GetType());
}

void MovementComponent::OnNewDestinationPosition(const CL_Pointf &oldValue, const CL_Pointf &newValue)
{
	ClearDestinationPositions();
	AddDestinationPosition(newValue);
	SendDestinationPositions();
}

void MovementComponent::Update(int deltaTime)
{
	GameObject *gameObject = (GameObject*)entity;
	
	//... perform some movement logic
}

void MovementComponent::ClearDestinationPositions()
{
	movementDestinations.clear();
}

void MovementComponent::AddDestinationPosition(const CL_Pointf &position)
{
	movementDestinations.push_back(position);
}

void MovementComponent::SendDestinationPositions()
{
	CL_NetGameEvent movementDestinationEvent(STC_OBJECT_MOVEMENT_LIST);

	for(size_t i = 0; i < movementDestinations.size(); ++i)
	{
		movementDestinationEvent.add_argument(movementDestinations[i].x);
		movementDestinationEvent.add_argument(movementDestinations[i].y);
	}

	GameObject* gameObject = (GameObject*)entity;
	gameObject->SendViewEvent(movementDestinationEvent);
}
--------------------------------------