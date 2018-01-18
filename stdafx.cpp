// stdafx.cpp : source file that includes just the standard includes
// IGRA_LAB.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

void SetMaterial(Material * m)
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
}
