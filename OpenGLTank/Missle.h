#pragma once
#include "Vector3f.h"

class Missle
{
public:
	Missle();
	~Missle();
	static void FireMissle(Vector3f startPosition, Vector3f startDirection, float tankSpeed);
	static void DrawAll();
	static float StartSpeed;
	static float Gravity;

private:
	static std::vector<Missle> missles;
	Vector3f position;
	Vector3f direction;
	float currentSpeed;

	GLUquadric *missleQuadratic;

	void DrawMissle();
};

