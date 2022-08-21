// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this is the header for the pyramid object

#pragma once
#ifndef PYRAMID_H
#define PYRAMID_H

class Pyramid
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

	Pyramid(GLFWwindow* passedWindow, GLfloat* verts, int sizeVerts);
};



#endif