#define _USE_MATH_DEFINES

#include <GL/glew.h>
#include <math.h>
#include <iostream>

namespace vGen
{

	void generateCircle(GLfloat *&vertices, GLushort *&indices, int verticesPerCircle, float radius, int* numVertices, int* numIndices);
	void generateCyllinder(GLfloat *&vertices, GLushort *&indices, int verticesPerCircle, float radius, int height, int* numVertices, int* numIndices);
}