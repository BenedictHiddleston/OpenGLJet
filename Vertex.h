#ifndef _VERTEX_H
#define _VERTEX_H

#ifdef LINUX
#include <GL/gl.h>
#endif

#ifdef WINDOWS
#include <Windows.h>
#endif

#ifdef OSX
#include <OpenGL/gl.h>
#endif



class Vertex
{
public:
	Vertex() ;
	Vertex(GLfloat a, GLfloat b, GLfloat c) ;

	GLfloat x, y, z ;
} ;

#endif