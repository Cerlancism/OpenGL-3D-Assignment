#include "stdafx.h"
#include "Environment.h"

Environment Environment::Global;
GLUquadric* skySphere;

Material skyMaterial =
{
	{ 80 / 256.0, 214 / 256.0, 255.0 / 256.0, 1.0 }, // Ambient
	{ 213 / 256.0, 256.0 / 256.0, 255.0 / 256.0, 1.0 }, // Diffuse
	{ 0.0, 0.0, 0.0, 1.0 }, // Specular
	8						// Shininess
};

Environment::Environment()
{
	skySphere = gluNewQuadric();
	gluQuadricDrawStyle(skySphere, GLU_FILL);
}

Environment::~Environment()
{}

void Environment::Draw()
{
	DrawGround();
	DrawSkyBox();
}

void Environment::DrawGround()
{
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture::GroundTexture.Image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	int areasize = 50;
	glPushMatrix();
	glScalef(2, 2, 2);
	glTranslatef(-6 * areasize /2, 0, -6 * areasize /2);
	for (int i = 0; i < areasize; i++)
	{
		glPushMatrix();
		for (int j = 0; j < 50; j++)
		{
			glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(3, 0, 3);
			glTexCoord2f(0, 1);
			glVertex3f(3, 0, -3);
			glTexCoord2f(1, 1);
			glVertex3f(-3, 0, -3);
			glTexCoord2f(1, 0);
			glVertex3f(-3, 0, 3);
			glEnd();
			glTranslatef(6, 0, 0);
		}
		glPopMatrix();
		glTranslatef(0, 0, 6);
	}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Environment::DrawSkyBox()
{
	glPushMatrix();
	gluQuadricNormals(skySphere, GLU_SMOOTH);
	glEnable(GL_LIGHTING);
	SetMaterial(&skyMaterial);
	gluSphere(skySphere, 1000, 128, 128);
	glDisable(GL_LIGHTING);
	glPopMatrix();
}
