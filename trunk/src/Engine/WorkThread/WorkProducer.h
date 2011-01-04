#pragma once

namespace Engine
{
	class WorkData;
	class WorkDoneData;
	class WorkProduction;
	class WorkProducer
	{
	public:
		WorkProducer();
		virtual ~WorkProducer();

		void insertProduction(WorkProduction *production) { this->production = production; }
		WorkProduction *getProduction() const { return production; }
		virtual void handle(WorkData *data) = 0;
		virtual void finished(WorkDoneData *data) = 0;

	private:
		WorkProduction *production;
	};
}
