/*
	Done by Algene.
*/

#include "stdafx.h"
#include "Missile.h"
#include "Vector3f.h"
#include <gl\GLU.h>

std::vector<Missile> Missile::missles;
float Missile::StartSpeed = Missile::MissileSpeed::MEDIUM;
float Missile::Gravity = 9.81f;
float Missile::CoolDown = 0.33f;
float Missile::lastFireTime = 0;

Missile::Missile()
{
	missleQuadratic = gluNewQuadric();
	gluQuadricDrawStyle(missleQuadratic, GLU_FILL);
}


Missile::~Missile()
{}
// Triggered when space bar press is detected implemented from tank.
void Missile::FireMissle(Vector3f startPosition, Vector3f startDirection, float tankSpeed)
{
	// Check for cool down time.
	if (Clock::GlobalClock.TimePassedSinceStartTime() - lastFireTime > CoolDown)
	{
		Debug::Log("Time passed: " + to_string(Clock::GlobalClock.TimePassedSinceStartTime()));
		Missile missle;
		missle.position = missle.startPosition = startPosition + startDirection * 0.5;
		missle.direction = startDirection;
		// The speed will inherit tank's speed and turrent direction.
		missle.currentSpeed = StartSpeed + tankSpeed * startDirection.Z;
		missle.currentSpeed = missle.currentSpeed < 5 ? 5 : missle.currentSpeed;
		// Track the start time for its use in trajectory fomula.
		missle.startTime = lastFireTime = Clock::GlobalClock.TimePassedSinceStartTime();
		missles.push_back(missle);
	}	
}

void Missile::DrawAll()
{
	// Wrapper method exposed statically to draw all the missiles in main file draw method.
	for (int i = 0; i < missles.size(); i++)
	{
		missles[i].DrawMissle();
	}
}

void Missile::DrawMissle()
{	
	// Transform only when above ground.
	if (position.Y > 0.3)
	{
		// Get the total time passed for the fired missile.
		double time = Clock::GlobalClock.TimePassedSinceStartTime() - startTime;
		// Keep track the position from the previous frame.
		Vector3f oldposition = position;
		// Apply appendix fomula for X and Z, the distance traveled.
		position.X = startPosition.X + currentSpeed * direction.X * cos(asin(direction.Y)) * time;
		position.Z = startPosition.Z + currentSpeed * direction.Z * cos(asin(direction.Y)) * time;
		// Apply appendix fomula for Y, the height of the missile against the ground.
		position.Y = startPosition.Y + currentSpeed * direction.Y * time - 0.5f * Gravity * time * time;
		// Use the new position and the previous frame position to get direction changed.
		Vector3f newdirection = (position - oldposition).Normalised();
		// Use the direction change to derive the pitch rotation of the missile.
		pitchAngleDirection = newdirection.Y;
		// Limit the lowest height so it does not enter underground.
		position.Y < 0.3 ? 0.3 : position.Y;
	}
	// Calculate the missile yaw and pitch angles in degree.
	float yawAngle = atan2(direction.X, direction.Z) * 180 / PI + 180;
	float pitchAngle = asin(pitchAngleDirection) * 180 / PI;
	// Draw the missile
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(position.X, position.Y, position.Z);
	glRotatef(yawAngle, 0, 1, 0);
	glRotatef(pitchAngle, 1, 0, 0);
	SetMaterial(&greenPlasticMaterial);
	gluCylinder(missleQuadratic, 0.05, 0.14, 0.5, 8, 8);
	int k = 0;
	int definition = 4;
	glBegin(GL_TRIANGLE_FAN);
	for (k = 0.0f; k <= definition * 2; k++) glVertex2f(0.05 * cos(3.142 * k / definition), 0.05 * sin(3.142 * k / definition));
	glEnd();
	glTranslatef(0, 0, 0.5);
	glBegin(GL_TRIANGLE_FAN);
	for (k = 0.0f; k <= definition * 2; k++) glVertex2f(0.14 * cos(3.142 * k / definition), 0.14 * sin(3.142 * k / definition));
	glEnd();
	glPopMatrix();
	glDisable(GL_LIGHTING);
}