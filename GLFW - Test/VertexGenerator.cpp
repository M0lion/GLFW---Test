#include "VertexGenerator.h"

void vGen::generateCircle(GLfloat *&vertices, GLushort *&indices, int verticesPerCircle, float radius, int* numVertices, int* numIndices)
{
	//generateCircle
	float radiansPerVertex = (M_PI * 2) / verticesPerCircle;
	vertices = new GLfloat[(verticesPerCircle + 1) * 4];
	vertices[0] = 0;
	vertices[1] = 0;
	vertices[2] = 0;
	vertices[3] = 1;

	float angle = 0;
	for (int i = 4; i + 3 < (verticesPerCircle + 1) * 4; i += 4)
	{
		vertices[i] = cos(angle) * radius;
		vertices[i + 1] = sin(angle) * radius;
		vertices[i + 2] = 0;
		vertices[i + 3] = 1;

		angle += radiansPerVertex;
	}

	indices = new GLushort[verticesPerCircle * 3];

	for (int i = 0; i < verticesPerCircle; i += 1)
	{
		indices[(i * 3)] = 0;
		indices[(i * 3) + 1] = i + 1;
		if (i < verticesPerCircle - 1)
			indices[(i * 3) + 2] = i + 2;
		else
			indices[(i * 3) + 2] = ((i + 2) % verticesPerCircle);
	}

	if (numIndices != 0)
		*numIndices = verticesPerCircle * 3;
	if (numVertices != 0)
		*numVertices = (verticesPerCircle + 1);
}

void vGen::generateCyllinder(GLfloat *&vertices, GLushort *&indices, int verticesPerCircle, float radius, int height, int* numVertices, int* numIndices)
{
	GLfloat *circle = 0;
	GLushort *unused = 0;

	generateCircle(circle, unused, verticesPerCircle, radius, numVertices, 0);

	vertices = new GLfloat[verticesPerCircle * 2 * 4];
	for (int i = 0; i + 3 < verticesPerCircle * 4; i+=4)
	{
		vertices[(i * 4)] = circle[(i * 4) + 1];
		vertices[(i * 4) + 1] = height / 2;
		vertices[(i * 4) + 2] = circle[(i * 4) + 2];
		vertices[(i * 4) + 3] = 1;
	}
	for (int i = verticesPerCircle * 4; i + 3 < verticesPerCircle * 2 * 4; i+=4)
	{
		vertices[(i)] = circle[(i) + 1];
		vertices[(i) + 1] = -height / 2;
		vertices[(i) + 2] = circle[(i) + 2];
		vertices[(i) + 3] = 1;
	}

	delete[](circle);
	delete[](unused);

	indices = new GLushort[verticesPerCircle * 3 * 2];
	for (int i = 0; i < verticesPerCircle * 3; i += 3)
	{
		indices[i] = i;
		indices[i + 1] = (verticesPerCircle + i) % (verticesPerCircle * 2);
		indices[i + 2] = (1 + i) % (verticesPerCircle);
	}
	for (int i = verticesPerCircle; i < verticesPerCircle * 3 * 2; i += 3)
	{
		indices[i] = (verticesPerCircle + i) % (verticesPerCircle * 2);;
		indices[i + 1] = i;
		indices[i + 2] = (verticesPerCircle + 1 + i) % (verticesPerCircle);
	}

	*numVertices = verticesPerCircle * 2;
	*numIndices = verticesPerCircle * 2;
}