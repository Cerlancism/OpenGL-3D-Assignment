#include "stdafx.h"
#include "Missle.h"
#include "Vector3f.h"
#include <gl\GL.h>
#include <gl\GLU.h>

Missle::Missle()
{
}


Missle::~Missle()
{
}

static void FireMissle(Vector3f position, Vector3f dir)
{
	DrawCylinder();
}

typedef struct materialStruct
{
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
} Material;

Material yellowPlasticMaterial = {
	{ 0.4, 0.4, 0.0, 1.0 }, // Ambient
	{ 0.9, 0.9, 0.0, 1.0 }, // Diffuse
	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
	8                      // Shininess
};

void SetMaterial(Material *m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
}

GLUquadric *myCylinder;

void DrawCylinder()
{
	glEnable(GL_LIGHTING);
	gluQuadricNormals(myCylinder, GLU_SMOOTH);
	SetMaterial(&yellowPlasticMaterial);
	gluCylinder(myCylinder, 1, 1, 10, 64, 64);
	glDisable(GL_LIGHTING);
}