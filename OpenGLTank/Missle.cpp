#include "stdafx.h"
#include "Missle.h"
#include "Vector3f.h"
#include <gl\GLU.h>

std::vector<Missle> Missle::missles;
float Missle::StartSpeed = 25;
float Missle::Gravity = 9.81f;
float time = 0;

Missle::Missle()
{
	missleQuadratic = gluNewQuadric();
	gluQuadricDrawStyle(missleQuadratic, GLU_FILL);
}


Missle::~Missle()
{}

void Missle::FireMissle(Vector3f startPosition, Vector3f startDirection, float tankSpeed)
{
	Missle missle;
	missle.position = startPosition + startDirection * 0.5;
	missle.direction = startDirection;
	missle.currentSpeed = StartSpeed + tankSpeed * abs(startDirection.Z);
	missle.currentSpeed = missle.currentSpeed < 10 ? 10 : missle.currentSpeed;
	missles.push_back(missle);
	missle.currentSpeed = 1;
}

void Missle::DrawAll()
{
	for (int i = 0; i < missles.size(); i++)
	{
		missles[i].DrawMissle();
	}
}

void Missle::DrawMissle()
{
	if (position.Y > 0)
	{
		time += Clock::DeltaTime;
		/*float newdirectY = direction.Y - Clock::DeltaTime * Gravity / 5;
		direction.Y = newdirectY < -0.8 ? -0.8 : newdirectY;
		position = position + direction * Clock::DeltaTime * currentSpeed;*/

		position.X += currentSpeed * cos(atan2(direction.Y, direction.X)) * time;
		position.Y += currentSpeed * sin(atan2(direction.Y, direction.X)) * time - 0.5f * Gravity * time * time;
		position.Z += currentSpeed * cos(atan2(direction.Y, direction.Z)) * time;

	}

	float yawAngle = asin(direction.X) * 180 / PI * (1 + asin(abs(direction.Y)));
	float pitchAngle = asin(direction.Y) * 180 / PI;
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(position.X, position.Y, position.Z);
	glRotatef(direction.Z < 0 ? 360 - yawAngle : 180 + yawAngle, 0, 1, 0);
	glRotatef(pitchAngle, 1, 0, 0);
	SetMaterial(&greenPlasticMaterial);
	gluCylinder(missleQuadratic, 0.05, 0.14, 0.5, 8, 8);
	int k = 0;
	int definition = 4;
	glBegin(GL_TRIANGLE_FAN);
	for (k = 0.0f; k <= definition * 2; k++)
		glVertex2f(0.05 * cos(3.142 * k / definition), 0.05 * sin(3.142 * k / definition));
	glEnd();
	glTranslatef(0, 0, 0.5);
	glBegin(GL_TRIANGLE_FAN);
	for (k = 0.0f; k <= definition * 2; k++)
		glVertex2f(0.14 * cos(3.142 * k / definition), 0.14 * sin(3.142 * k / definition));

	glEnd();
	glPopMatrix();
	glDisable(GL_LIGHTING);
}