#pragma once

#include "ITimer.h"
#include <windows.h>
#include <iostream>

namespace Engine
{

class Win32Timer : public ITimer
{
public:
	Win32Timer();
	virtual ~Win32Timer();

	virtual void start();
	virtual void stop();
	virtual double update();
	virtual double getTimeElapsed();
	virtual double getTimeSinceStart();
	virtual double getFps();

private:
	_LARGE_INTEGER frequency;
	_LARGE_INTEGER time;

	double time_start;
	double time_lastupdate;
	double time_currentelapse;
	double time_accumulated;

	unsigned int framecount;
	unsigned int framecount_snapshot;
	double time_snapshot;
	double fps;

	bool firstcall;
	bool stopped;
};

}
