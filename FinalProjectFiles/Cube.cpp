// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// the definitions and logic for the pyramid object

// TODO catch the error that may occur if the mesh fails to create

#include <iostream>         // cout, cerr etc.
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

#include "Cube.h"

using namespace std;

// default constructor (passed window, passed verticies)
Cube::Cube(GLFWwindow* passedWindow, GLfloat* verts, int sizeVerts) {
	this->Window = passedWindow;
	Cube::CreateMesh(gMesh, verts, sizeVerts);
}

void Cube::CreateMesh(GLMesh& mesh, GLfloat* verts, int sizeVerts)
{
	//########################################################################################################################################
	// Mesh starts here
	//########################################################################################################################################

	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(2, mesh.vbos);	// create buffers
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // activate buffer
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW); // send vertex data

	// creates a buffer for the indices
	GLushort indices[] = {
		 0,  1,  3, // Front face
		 3,  1,  2, // Front face

		 4,  5,  7, // Left face
		 7,  5,  6, // Left face 

		 8,  9, 11, // Back face
		11,  9, 10, // Back face 

		12, 13, 15, // Right face
		15, 13, 14, // Right face

		16, 17, 19, // Top face
		19, 17, 18, // Top face

		20, 21, 23, // Bottom face 
		23, 21, 22, // Bottom face
	};
	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// specify numbers per vertex, numbers per color, and numbers per uv map
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 4; //(R, G, B, A)
	const GLuint floatsPerUV = 2;
	const GLuint floatsPerNormal = 3;

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV + floatsPerNormal);

	// create vertex attribute pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, floatsPerColor, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerColor)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV)));
	glEnableVertexAttribArray(3);

}


//delete mesh
void Cube::DestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}