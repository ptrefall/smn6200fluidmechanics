/*Component-based Entity Engine
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
*/

#pragma once

#include "ComponentContainer.h"
#include "PropertyContainer.h"
#include "ComponentFactory.h"

namespace Engine
{
class CoreMgr;

class IEntity : public ComponentContainer, public PropertyContainer
{
public:
	IEntity(unsigned int id, const CL_String &type, const CL_String &name, CoreMgr *coreMgr, ComponentFactory &factory)
		: ComponentContainer(coreMgr, factory), PropertyContainer(coreMgr)
	{
		this->id = id;
		this->type = type;
		this->name = name;
		this->coreMgr = coreMgr;
	}

	virtual ~IEntity();

	virtual void Update(float dt) {}
	virtual void Render() {}

	unsigned int getId() const { return id; }
	const CL_String &getType() const { return type; }
	const CL_String &getName() const { return name; }
	virtual CL_String getSpecialType() const { return CL_String(); }

protected:
	unsigned int id;
	CL_String type;
	CL_String name;
	CoreMgr *coreMgr;
};
}
