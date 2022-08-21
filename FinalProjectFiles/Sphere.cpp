// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this contains the logic and definitions for the sphere object
// The sphere is drawn in bands around the circle with two for loops

#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library
#include "Sphere.h"

using namespace std;

Sphere::Sphere(int precision)
{
	// set the number of verticies and indicies based on the given precision
	// the +1s are needed to duplicate the last verts and give different uvs so
	// the texture wraps correctly
	numVertices = (precision+ 1) * (precision + 1);
	numIndices = precision * precision * 6;

	// resize the vectors to the right size pushing back empty data to allocate
	for (int i = 0; i < numVertices; i++)
	{
		vertices.push_back(glm::vec3());
	}
	for (int i = 0; i < numVertices; i++)
	{
		texCoords.push_back(glm::vec2());
	}
	for (int i = 0; i < numVertices; i++)
	{
		normals.push_back(glm::vec3());
	}
	for (int i = 0; i < numIndices; i++)
	{
		indices.push_back(0);
	}

	// calculate tri verticies, normals, and textureCoordinates
	// the first loop is for the sphere the second is for a band going around
	// the sphere. loop around the band then jump up and loop around the next band
	// drawing the entire sphere.
	for (int i = 0; i <= precision; i++)
	{
		for (int j = 0; j <= precision; j++)
		{
			// calculate x, y, and z
			float y = (float)cos(toRadians(180.0f - i * 180.0f / precision));
			float x = -(float)cos(toRadians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));
			float z = (float)sin(toRadians(j * 360.0f / precision)) * (float)abs(cos(asin(y)));

			// set the verticies, texture coordinates, and normals into the vectors
			vertices[i * (precision + 1) + j] = glm::vec3(x, y, z);
			texCoords[i * (precision + 1) + j] = glm::vec2(((float)j / precision), ((float)i / precision));
			normals[i * (precision + 1) + j] = glm::vec3(x, y, z);
		}
	}

	// calculate tri indices
	for (int i = 0; i < precision; i++)
	{
		for (int j = 0; j < precision; j++)
		{
			indices[6 * (i * precision + j)] = i * (precision + 1) + j;
			indices[6 * (i * precision + j) + 1] = i * (precision + 1) + j + 1;
			indices[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
			indices[6 * (i * precision + j) + 3] = i * (precision + 1) + j + 1;
			indices[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
			indices[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
		}
	}

	// test prints that I used to check the math and what was being stored
	
	//for (int i = 0; i < numIndices; i++)
	//{
	//	cout << indices[i] << endl;
	//}
	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	cout << "PointCoords" << i << " : x: " << vertices[i].x << " y: " << vertices[i].y << " z: " << vertices[i].z << endl;
	//	cout << "PointTextureCoords" << i << " : x: " << texCoords[i].x << " y: " << texCoords[i].y << " z: " << endl;
	//	cout << "PointNormals" << i << " : x: " << normals[i].x << " y: " << normals[i].y << " z: " << normals[i].z << endl;
	//	cout << "----------------------------------------------------------------------------------------------------------------" << endl;
	//}

	// create the mesh from the calculated data
	Sphere::createMesh(gMesh);
}

void Sphere::createMesh(GLMesh& mesh)
{
	// vector that stores the vertices, texture coordinates, and normals
	std::vector<float> vertsCombined;

	// store the data in one vector
	for (int i = 0; i < vertices.size(); i++)
	{
		vertsCombined.push_back(vertices[i].x); // get and add x
		vertsCombined.push_back(vertices[i].y); // get and add y
		vertsCombined.push_back(vertices[i].z); // get and add z

		vertsCombined.push_back(texCoords[i].x); // texture coord x
		vertsCombined.push_back(texCoords[i].y); // texture coord y

		vertsCombined.push_back(normals[i].x);  // normal coord x
		vertsCombined.push_back(normals[i].y);  // normal coord y
		vertsCombined.push_back(normals[i].z);  // normal coord z

	}

	mesh.nIndices = indices.size();
	glGenVertexArrays(1, &mesh.vao);
	glGenBuffers(3, mesh.vbos);
	glGenBuffers(1, &mesh.ebo);

	// buffer for vertices, texture, and normals
	glBindVertexArray(mesh.vao);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, vertsCombined.size() * sizeof(float), vertsCombined.data(), GL_STATIC_DRAW); // send vertex data

	// buffer for indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// specify numbers per vertex, numbers per color, and numbers per uv map
	const GLuint floatsPerVertex = 3;
	const GLuint floatsPerUV = 2;
	const GLuint floatsPerNormal = 3;

	// Strides between vertex coordinates
	GLint stride = sizeof(float) * (floatsPerVertex + floatsPerUV + floatsPerNormal);

	// create vertex attribute pointers
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(2, floatsPerUV, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * floatsPerVertex));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, floatsPerNormal, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * (floatsPerVertex + floatsPerUV)));
	glEnableVertexAttribArray(3);

	// unbind buffers and vertex array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


// this is used to convert to radians I made a method so it is easier
float Sphere::toRadians(float degrees)
{
	return degrees * (PI / 180.0f);
}