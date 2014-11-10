#ifndef _NORMAL_H
#define _NORMAL_H

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

class Normal: public Vertex
{
public:
	Normal() ;
	Normal(GLfloat a, GLfloat b, GLfloat c) ;

} ;

#endif