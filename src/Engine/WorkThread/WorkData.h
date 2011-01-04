#pragma once

namespace Engine
{
	class WorkProducer;
	class WorkData
	{
	public:
		WorkData();
		virtual ~WorkData();

		virtual void handle() = 0;
		virtual bool isHandled() = 0;

		virtual WorkProducer *getProducer() const { return NULL; }
	};
}
