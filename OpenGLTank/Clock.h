#pragma once

class Clock
{
public:
	static float DeltaTime;

	static Clock GlobalClock;

	Clock();

	static void Start();
	static double Refresh();
	static double TimePassedSinceStartTime();

	~Clock();
};

