#include "stdafx.h"
#include "Clock.h"


Clock::Clock()
{

}

void Clock::Start()
{
	__int64 currentTimeInCounts;
	// countsPerSecond depends on your PC
	if (!QueryPerformanceFrequency(
		(LARGE_INTEGER *)&countsPerSecond))
	{
		MessageBox(NULL, L"QueryPerformanceFrequency Failed.", L"ERROR", MB_OK | MB_ICONINFORMATION);
		return;
	}
	QueryPerformanceCounter(
		(LARGE_INTEGER *)&currentTimeInCounts);
	startTimeInCounts = currentTimeInCounts;
	lastTimeInCounts = currentTimeInCounts;
}

double Clock::Refresh()
{
	__int64 currentTimeInCounts, timePassedSinceLastTimeInCounts;
	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSinceLastTimeInCounts =
		currentTimeInCounts - lastTimeInCounts;
	double timePassedSinceLastTimeInSeconds =
		(currentTimeInCounts - lastTimeInCounts) /
		(double)countsPerSecond;
	lastTimeInCounts = currentTimeInCounts;
	DeltaTime = timePassedSinceLastTimeInSeconds;
	return timePassedSinceLastTimeInSeconds;
}

double Clock::TimePassedSinceStartTime()
{
	__int64 currentTimeInCounts;
	double timePassedSeconds;
	// Calculate time passed in seconds since timer was started
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSeconds = (currentTimeInCounts - startTimeInCounts) /
		(double)countsPerSecond;
	return timePassedSeconds;
}


Clock::~Clock()
{}
