// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// the definitions and logic for the plane object

#include <iostream>         // cout, cerr etc.
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

#include "Plane.h"

using namespace std;

// default constructor (passed window, window height, window width, passed verticies, passed alpha)
Plane::Plane(GLFWwindow* passedWindow, GLfloat* verts, int sizeVerts) {
	this->Window = passedWindow;
	Plane::CreateMesh(gMesh, verts, sizeVerts);
}

void Plane::CreateMesh(GLMesh& mesh, GLfloat* verts, int sizeVerts)
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
	// to make one color inicies are more numerous
	GLushort indices[] = {
		0, 1, 2, //First triangle face
		0, 2, 3, //Second triangle face 
	};
	mesh.nIndices = sizeof(indices) / sizeof(indices[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vbos[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// specify numbers per vertex and numbers per color
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerColor = 4; //(R, G, B, A)
	const GLuint floatsPerUV = 2;
	const GLuint floatsPerNormal = 3;

	// Strides between vertex coordinates is 6
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerColor + floatsPerUV + floatsPerNormal);

	// create vertex attribute pointer
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
void Plane::DestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}