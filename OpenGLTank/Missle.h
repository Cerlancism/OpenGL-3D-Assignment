#pragma once
#include "Vector3f.h"

class Missle
{
public:
	Missle();
	~Missle();
	static void FireMissle(Vector3f position, Vector3f dir);

private:
	void DrawCylinder();
};

