#include "precomp.h"
#include "GuiProjectTableEditFormatter.h"

using namespace Engine;

GuiProjectTableEditFormatter::GuiProjectTableEditFormatter()
: Rocket::Controls::DataFormatter("edit")
{
}

GuiProjectTableEditFormatter::~GuiProjectTableEditFormatter()
{
}

void GuiProjectTableEditFormatter::FormatData(Rocket::Core::String& formatted_data, const Rocket::Core::StringList& raw_data)
{
	// Data format:
	// raw_data[0] is the image filename
	formatted_data = "<button style=\"width: 18px; height: 24px;\" onclick=\"ProjectWindow:OnEditClicked(Entities[" + raw_data[0] + "],\"><img src=\"edit_small.tga\" /></button>";
}
