#include "stdafx.h"
#include "Environment.h"

Environment Environment::Global;

Environment::Environment()
{

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
{}
