#include "stdafx.h"
#include "Missle.h"
#include "Vector3f.h"
#include <gl\GL.h>
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

Material yellowPlasticMaterial =
{
	{ 0.4, 0.4, 0.0, 1.0 }, // 
	{ 0.9, 0.9, 0.0, 1.0 }, // Diffuse
	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
	8                       // Shininess
};

GLUquadric *myCylinder;

void Missle::DrawCylinder()
{
	glEnable(GL_LIGHTING);
	gluQuadricNormals(myCylinder, GLU_SMOOTH);
	SetMaterial(&yellowPlasticMaterial);
	gluCylinder(myCylinder, 1, 1, 10, 64, 64);
	glDisable(GL_LIGHTING);
}