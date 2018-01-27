#pragma once

class Missile
{
public:
	enum MissileSpeed { SLOW = 8, MEDIUM = 15, FAST = 33 };

	Missile();
	~Missile();
	static void FireMissle(Vector3f startPosition, Vector3f startDirection, float tankSpeed);
	static void DrawAll();
	static float StartSpeed;
	static float Gravity;
	static float CoolDown;

private:
	static std::vector<Missile> missles;
	static float lastFireTime;
	Vector3f startPosition;
	Vector3f position;
	Vector3f direction;
	float currentSpeed;
	float pitchAngleDirection = 0;
	float startTime;

	GLUquadric *missleQuadratic;

	void DrawMissle();
};

