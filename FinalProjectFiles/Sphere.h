// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this is the header for the sphere object

#pragma once
#ifndef SPHERE_H
#define SPHERE_H
#include <vector>

class Sphere
{
private:

	// stores the Gl data for a mesh
	struct GLMesh
	{
		GLuint vao; // Handles for vertex array object
		GLuint vbos[3]; // Handles for vertex buffer object
		GLuint nIndices; // number of indices
		GLuint ebo;
	};

	double const PI = 3.14159265359;
	int numVertices;
	int numIndices;
	GLfloat verts;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector < glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	float toRadians(float degrees);
	void createMesh(GLMesh& mesh);

public:
	Sphere(int precision);
	GLMesh gMesh;
};

#endif