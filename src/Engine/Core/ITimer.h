#pragma once

namespace Engine
{

class ITimer
{
public:
	virtual ~ITimer() {}

	virtual void start() = 0;
	virtual void stop() = 0;
	virtual double update() = 0;
	virtual double getTimeElapsed() = 0;
	virtual double getTimeSinceStart() = 0;
	virtual double getFps() = 0;
};

}
