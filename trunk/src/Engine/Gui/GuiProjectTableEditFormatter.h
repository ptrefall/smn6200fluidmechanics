#pragma once

#include <Rocket/Controls/DataFormatter.h>

namespace Engine
{
	class GuiProjectTableEditFormatter : public Rocket::Controls::DataFormatter
	{
	public:
		GuiProjectTableEditFormatter();
		virtual ~GuiProjectTableEditFormatter();

		virtual void FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data);
	};
}
