#pragma once

class Clock
{
public:
	__int64 startTimeInCounts = 0;
	__int64 lastTimeInCounts = 0;
	__int64 countsPerSecond;
	double DeltaTime;

	Clock();

	void Start();
	double Refresh();
	double TimePassedSinceStartTime();

	~Clock();
};

