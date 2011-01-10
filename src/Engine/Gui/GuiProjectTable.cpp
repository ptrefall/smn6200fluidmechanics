#include "precomp.h"
#include "GuiProjectTable.h"
#include <Entity/IEntity.h>

using namespace Engine;

GuiProjectTable::GuiProjectTable()
: Rocket::Controls::DataSource("project_table")
{
}

GuiProjectTable::~GuiProjectTable()
{
}

void GuiProjectTable::GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns)
{
	if (table == "entities")
	{
		for (size_t i = 0; i < columns.size(); i++)
		{
			if (columns[i] == "name")
			{
				row.push_back(entities[row_index].name);
			}
			else if (columns[i] == "name_required")
			{
				row.push_back(Rocket::Core::String(4, "%d", entities[row_index].name_required));
			}
			else if (columns[i] == "type")
			{
				row.push_back(entities[row_index].type);
			}
		}
	}
}

int GuiProjectTable::GetNumRows(const Rocket::Core::String& table)
{
	if (table == "entities")
		return (int)entities.size();

	return 0;
}

void GuiProjectTable::addEntity(IEntity *entity)
{
	EntityStruct sEntity;
	sEntity.name_required = true;
	sEntity.name = entity->getName().c_str();
	sEntity.type = entity->getType().c_str();
	entities.push_back(sEntity);
	
	NotifyRowAdd("entities", entities.size()-1, 1);
}
