// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// This is the header for the shader file

#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "Pyramid.h"
#include "Camera.h"
#include "Plane.h"
#include "Icosahedron.h"
#include "Cube.h"
#include "Sphere.h"
#include <glm/glm.hpp>

bool initShader(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId);
void deleteShader(GLuint programId);
void render(Pyramid& pyramid, float alpha, Camera& cam, GLuint textureId);
void render(Pyramid& pyramid, float alpha, Camera& cam, GLuint textureId, GLuint texture2Id);
void render(Plane& plane, float alpha, Camera& cam, GLuint textureId);
void render(Icosahedron& ico, float alpha, Camera& cam, GLuint textureId);
void render(Cube& cube, float alpha, Camera& cam, GLuint textureId);
void render(Sphere& sphere, float alpha, Camera& cam, GLuint textureId);
bool createShader();
GLuint getProgramId();
bool loadTexture(const char* filename, GLuint& textureId);
void deleteTexture(GLuint textureId);

#endif