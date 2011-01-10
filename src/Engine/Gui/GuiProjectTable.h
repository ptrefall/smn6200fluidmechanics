#pragma once

#include <Rocket/Controls/DataSource.h>

namespace Engine
{
	class IEntity;

	class GuiProjectTable : public Rocket::Controls::DataSource
	{
	public:
		GuiProjectTable();
		virtual ~GuiProjectTable();

		virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);
		virtual int GetNumRows(const Rocket::Core::String& table);

		void addEntity(IEntity *entity);

	private:
		struct EntityStruct
		{
			Rocket::Core::String name;
			unsigned int id;
		};
		std::vector<EntityStruct> entities;
	};
}
