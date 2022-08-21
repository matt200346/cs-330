// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this is the header for the plane object

#pragma once
#ifndef ICOSAHEDRON_H
#define ICOSAHEDRON_H

class Icosahedron
{
private:

	// stores the Gl data for a mesh
	struct GLMesh
	{
		GLuint vao; // Handles for vertex array object
		GLuint vbos[2]; // Handles for vertex buffer object
		GLuint nIndices; // number of indices
	};

	GLFWwindow* Window;

	void DestroyMesh(GLMesh& mesh);
	void CreateMesh(GLMesh& mesh, GLfloat* verts, int sizeVerts);

public:
	GLMesh gMesh;

	Icosahedron(GLFWwindow* passedWindow, GLfloat* verts, int sizeVerts);
};



#endif