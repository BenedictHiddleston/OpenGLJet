#include "Face.h"

Face::Face()
{
	x = Vertex() ;
	y = Vertex() ;
	z = Vertex() ;
}

Face::Face(Vertex a, Vertex b, Vertex c, Normal na, Normal nb, Normal nc, int mat_inx) 
{
	x = a ;
	y = b ;
	z = c ;
	nx = na ;
	ny = nb ;
	nz = nc ;
	mat_inx = mat_inx ;
}