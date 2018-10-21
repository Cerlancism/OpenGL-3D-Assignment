#pragma once

class Clock
{
public:
	static float DeltaTime;

	static Clock GlobalClock;

	void Start();
	double Refresh();
	double TimePassedSinceStartTime();

	

private:
	Clock();
	~Clock();
};

