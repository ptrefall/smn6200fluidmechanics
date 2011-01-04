#pragma once

#include "ResFactory.h"

#include "XMLResource.h"

namespace Engine
{
	class ResReg
	{
	public:
		static void Register(ResFactory *factory)
		{
			factory->registerResource(XMLResource::getType(), &XMLResource::create);
		}
	};
}
