#include "stdafx.h"
#include "Missle.h"
#include "Vector3f.h"
#include <gl\GLU.h>

Missle::Missle()
{}


Missle::~Missle()
{}

void Missle::FireMissle(Vector3f position, Vector3f dir)
{
	Missle missle;
	missle.DrawCylinder();
}

GLUquadric *myCylinder;

void Missle::DrawCylinder()
{
	glEnable(GL_LIGHTING);
	gluQuadricNormals(myCylinder, GLU_SMOOTH);
	SetMaterial(&yellowPlasticMaterial);
	gluCylinder(myCylinder, 1, 1, 10, 64, 64);
	glDisable(GL_LIGHTING);
}