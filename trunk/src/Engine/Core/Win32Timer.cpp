#include "precomp.h"
#include "Win32Timer.h"

using namespace Engine;

Win32Timer::Win32Timer()
{
	time_start = 0.0;
	time_lastupdate = 0.0;
	time_currentelapse = 0.0;
	time_accumulated = 0.0;

	time_snapshot = 0.0;

	firstcall = true;
	stopped = false;
}

Win32Timer::~Win32Timer()
{
}

void Win32Timer::start()
{
	if(firstcall)
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&time);
		time_start = time.QuadPart*(1000000.0 / frequency.QuadPart) * 0.000001;
		time_lastupdate = time_start;

		firstcall = false;
	}
	stopped = false;
}

void Win32Timer::stop()				// need some work
{
	stopped = true;
}

double Win32Timer::update()
{
	double time_previous = time_lastupdate;
	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&frequency);
	time_lastupdate = time.QuadPart*(1000000.0 / frequency.QuadPart) * 0.000001;

	time_currentelapse = time_lastupdate - time_previous;
	time_accumulated += time_currentelapse;

	double delta_time = time_accumulated - time_snapshot;
	if( delta_time > 0.5 )
	{
		fps = (framecount - framecount_snapshot) / delta_time;
		time_snapshot = time_accumulated;
		framecount_snapshot = framecount;
	}
	framecount++;

	return time_currentelapse;
}

double Win32Timer::getTimeElapsed()
{
	return time_currentelapse;
}

double Win32Timer::getTimeSinceStart()
{
	return time_accumulated;
}

double Win32Timer::getFps()
{
	return fps;
}
