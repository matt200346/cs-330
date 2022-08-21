// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// the definitions and logic for the icosahedron/sphere object
// to make it into a sphere we simply have to subdivide the faces

// TODO catch the error that may occur if the mesh fails to create

#include <iostream>         // cout, cerr etc.
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

#include "icosahedron.h"

using namespace std;

// default constructor (passed window, passed verticies)
Icosahedron::Icosahedron(GLFWwindow* passedWindow, GLfloat* verts, int sizeVerts) {
	this->Window = passedWindow;
	Icosahedron::CreateMesh(gMesh, verts, sizeVerts);
}

void Icosahedron::CreateMesh(GLMesh& mesh, GLfloat* verts, int sizeVerts)
{
	//########################################################################################################################################
	// Mesh starts here
	//########################################################################################################################################

	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(2, mesh.vbos);	// create buffers
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]); // activate buffer
	glBufferData(GL_ARRAY_BUFFER, sizeVerts, verts, GL_STATIC_DRAW); // send vertex data

	// creates a buffer for the indices 20 tris for an icosahedron
	GLushort indices[] = {
		0,1,2,
		3,4,5,
		6,7,8,
		9,10,11,
		12,13,14,
		15,16,17,
		18,19,20,
		21,22,23,
		24,25,26,
		27,28,29,
		30,31,32,
		33,34,35,
		36,37,38,
		39,40,41,
		42,43,44,
		45,46,47,
		48,49,50,
		51,52,53,
		54,55,56,
		57,58,59
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
void Icosahedron::DestroyMesh(GLMesh& mesh)
{
	glDeleteVertexArrays(1, &mesh.vao);
	glDeleteBuffers(2, mesh.vbos);
}