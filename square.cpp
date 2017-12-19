#include "stdafx.h"
#include "square.h"
#include <gl\GL.h>
#include <gl\GLU.h>

Square::Square()
{
	left = -0.2;
	top = 0.2;
	width = 0.4;
	height = 0.4;
	isSelected = false;
}


void Square::Draw()
{
	if (isSelected)
	{
		glColor3f(0, 0, 1);
	}
	else
	{
		glColor3f(0, 0, 0);
	}

	glBegin(GL_LINE_LOOP);
	{
		glLineWidth(0.0f);
		glVertex2f(left, top);
		glVertex2f(left + width, top);
		glVertex2f(left + width, top - height);
		glVertex2f(left, top - height);
	}
	glEnd();
}

bool ptInRect(float x, float y, float left, float top, float width, float height)
{
	return (x >= left && x <= left + width && y >= top - height && y <= top);
}

void Square::HandleLButtonDown(float xpos, float ypos)
{
	isSelected = ptInRect(xpos, ypos, left, top, width, height);
}

void Square::HandleMouseMove(float xpos, float ypos)
{
	left = xpos;
	top = ypos;
}


Square::~Square()
{}
