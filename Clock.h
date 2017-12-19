#pragma once

class Clock
{
public:
	__int64 startTimeInCounts = 0;
	__int64 lastTimeInCounts = 0;
	__int64 countsPerSecond;

	Clock();

	void Start();
	double DeltaTime();
	double TimePassedSinceStartTime();

	~Clock();
};

