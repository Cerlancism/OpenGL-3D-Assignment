#pragma once

class Clock
{
public:
	static float DeltaTime;

	static Clock GlobalClock;

	Clock();

	void Start();
	double Refresh();
	double TimePassedSinceStartTime();

	~Clock();
};

