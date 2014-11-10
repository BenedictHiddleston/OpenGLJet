#ifndef _FACE_H
#define _FACE_H


#ifdef LINUX
#include <GL/gl.h>
#endif

#ifdef WINDOWS
#include <Windows.h>
#endif

#ifdef OSX
#include <OpenGL/gl.h>
#endif

# include "Normal.h"
// # include "Vertex.h"

class Face
{
public:
	Face() ;
	Face(Vertex a, Vertex b, Vertex c, Normal na, Normal nb, Normal nc, int mat_inx) ;

	Vertex x, y, z ;
	Normal nx, ny, nz ;
	int mtl ;
};

#endif