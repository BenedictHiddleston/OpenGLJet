#ifndef _MATERIAL_H
#define _MATERIAL_H

#ifdef LINUX
#include <GL/gl.h>
#endif

#ifdef WINDOWS
#include <Windows.h>
#endif

#ifdef OSX
#include <OpenGL/gl.h>
#endif

#include "Vertex.h"

class Material
{
public:
	Material();

	GLfloat sr, sg, sb;
	GLfloat ar, ag, ab;
	GLfloat dr, dg, db;
	GLfloat er, eg, eb;
	GLfloat transparency, shiny;

	int mtl_inx;	
};

#endif