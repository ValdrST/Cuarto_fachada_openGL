#pragma once

#include <glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateMesh(GLfloat *vertices, GLfloat *textcoords, GLfloat *normals, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfTexcoords, unsigned int numOfNormals, unsigned int numOfIndices);
	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

