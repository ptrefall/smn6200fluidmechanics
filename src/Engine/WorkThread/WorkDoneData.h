#pragma once

namespace Engine
{
	class WorkDoneData
	{
	public:
		WorkDoneData();
		virtual ~WorkDoneData();

		virtual void handle() = 0;
	};
}
