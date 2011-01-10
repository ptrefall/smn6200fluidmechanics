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
			else if (columns[i] == "id")
			{
				row.push_back(Rocket::Core::String(32, "%d", entities[row_index].id));
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
	sEntity.name = entity->getName().c_str();
	sEntity.id = entity->getId();
	entities.push_back(sEntity);
	
	NotifyRowAdd("entities", entities.size()-1, 1);
}
