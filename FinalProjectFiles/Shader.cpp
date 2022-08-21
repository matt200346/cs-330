// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this file contains the functions and logic of the shader

// TODO change the arguments passed to render() so x, y, and z rotation can be specified directly

#include <iostream>         // cout, cerr etc.
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Plane.h"
#include "Icosahedron.h"
#include "Sphere.h"

// needed for textures
#define STB_IMAGE_IMPLEMENTATIION
#include "stb_image.h"

// Shader program Macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version " core \n" #Source
#endif

GLuint gProgramId;
GLuint gLampId;

//light color and object color
glm::vec3 gObjectColor(1.0f, 1.0f, 0.0f);
glm::vec3 gLight1Color(1.0f, 1.0f, 0.8f); // Light skewed more towards yellow
glm::vec3 gLight2Color(0.2f, 0.5f, 1.0f); // Blue light

// light 1 postion and scale variables
glm::vec3 gLight1Pos(3.5f, 2.5f, 4.5f);
glm::vec3 gLight1Scale(0.5f);

// light 2 postion and scale variables
glm::vec3 gLight2Pos(-4.5f, 1.5f, -1.0f);
glm::vec3 gLight2Scale(0.5f);

// vertex shader
// Object vertex shader
const GLchar* objectVertexShaderSource = GLSL(440,
	layout(location = 0) in vec3 position;
	layout(location = 3) in vec3 normal;
	layout(location = 2) in vec2 textureCoordinate;

	out vec3 vertexNormal;
	out vec3 vertexFragmentPos;
	out vec2 vertexTextureCoordinate;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()
	{
		gl_Position = projection * view * model * vec4(position, 1.0f);
		vertexFragmentPos = vec3(model * vec4(position, 1.0f));
		vertexNormal = mat3(transpose(inverse(model))) * normal;
		vertexTextureCoordinate = textureCoordinate;
	}
);

// Object fragment shader
const GLchar* objectFragmentShaderSource = GLSL(440,
	in vec3 vertexNormal;
	in vec3 vertexFragmentPos;
	in vec2 vertexTextureCoordinate;
	
	out vec4 fragmentColor;

	uniform vec3 objectColor;
	uniform vec3 light1Color;
	uniform vec3 light1Pos;
	uniform vec3 light2Color;
	uniform vec3 light2Pos;
	uniform vec3 viewPos;
	uniform sampler2D uTexturebase;
	uniform sampler2D uTextureExtra;
	uniform bool multipleTextures;

	void main()
	{
		// calculate ambient lighting
		float ambientStrength = 0.2f;
		vec3 ambient = ambientStrength * light1Color * light2Color;

		// calculate the diffuse lighting
		vec3 norm = normalize(vertexNormal);

		// light one diffuse
		vec3 light1Direction = normalize(light1Pos - vertexFragmentPos);
		float light1Impact = max(dot(norm, light1Direction), 0.0);
		vec3 light1Diffuse = light1Impact * 1.5 * light1Color;

		// light 2 diffuse
		vec3 light2Direction = normalize(light2Pos - vertexFragmentPos);
		float light2Impact = max(dot(norm, light2Direction), 0.0);
		vec3 light2Diffuse = light2Impact * 0.8 * light2Color;

		// total diffuse
		vec3 diffuse = light1Diffuse + light2Diffuse;

		// calculate the specular lighting
		float light1SpecularIntensity = 1.7f;
		float light1HighlightSize = 50.0f;
		float light2SpecularIntensity = 1.7f;
		float light2HighlightSize = 50.0f;

		vec3 viewDir = normalize(viewPos - vertexFragmentPos);

		// reflection direction
		vec3 light1ReflectDir = reflect(-light1Direction, norm);
		vec3 light2ReflectDir = reflect(-light2Direction, norm);

		// calculate specular component
		float light1SpecularComponent = pow(max(dot(viewDir, light1ReflectDir), 0.0), light1HighlightSize);
		vec3 light1Specular = light1SpecularIntensity * light1SpecularComponent * light1Color;
		float light2SpecularComponent = pow(max(dot(viewDir, light2ReflectDir), 0.0), light2HighlightSize);
		vec3 light2Specular = light2SpecularIntensity * light2SpecularComponent * light2Color;

		vec3 specular = light1Specular + light2Specular;

		// set texture color
		vec4 textureColor = texture(uTexturebase, vertexTextureCoordinate);
		if (multipleTextures)
		{
			vec4 extraTexture = texture(uTextureExtra, vertexTextureCoordinate);
			if (extraTexture.a != 0.0)
				textureColor = extraTexture;
		}

		// set phong
		vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;
		fragmentColor = vec4(phong, 1.0);
		//fragmentColor = vec4(vertexTextureCoordinate.x, vertexTextureCoordinate.y, 0, 1);
	}
);

// Lamp vertex shader
const GLchar* lampVertexShaderSource = GLSL(440,

	layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
}
);


// lamp fragment shader
const GLchar* lampFragmentShaderSource = GLSL(440,

	out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(1.0f);
}
);


// function that main can call to initialize shader (might mess with in the future)
bool createShader()
{
	// if shader fails to initialize then return false
	if (!initShader(objectVertexShaderSource, objectFragmentShaderSource, gProgramId))
	{
		return false;
	}

	// if shader fails to initialize then return false
	if (!initShader(lampVertexShaderSource, lampFragmentShaderSource, gLampId))
	{
		return false;
	}

	// else return true, since it succeeded
	return true;
}

// initialize shader
bool initShader(const char* vtxShaderSource, const char* fragShaderSource, GLuint& programId)
{
	// error reporting variables
	int success = 0;
	char infoLog[512];

	// shader program object
	programId = glCreateProgram();

	// vertex and fragment shader objects
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	// get shader source
	glShaderSource(vertexShaderId, 1, &vtxShaderSource, NULL);
	glShaderSource(fragmentShaderId, 1, &fragShaderSource, NULL);

	// compile vertex shader
	glCompileShader(vertexShaderId);

	// check for errors
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "Vertex shader failure!!!!\n" << infoLog << std::endl;

		return false;
	}

	// compile fragment shader
	glCompileShader(fragmentShaderId);

	// check for errors
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, sizeof(infoLog), NULL, infoLog);
		std::cout << "Fragment shader failure!!!!\n" << infoLog << std::endl;

		return false;
	}

	// Attach shaders
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	//check to make sure no linking errors occured
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programId, sizeof(infoLog), NULL, infoLog);
		std::cout << "Linking shader failure!!!!\n" << infoLog << std::endl;

		return false;
	}

	// use shader program
	glUseProgram(programId);
	return true;
}

// delete the shader
void deleteShader(GLuint programId)
{
	glDeleteProgram(programId);
}

// render the frame for a pyramid object
// TODO add the abillity to pass values to change the scale, translation, and the directions of rotation
void render(Pyramid &pyramid, float alpha, Camera &cam, GLuint textureId)
{
//########################################################################################################################################
// Render starts here
//########################################################################################################################################

	// ---------------------------------------------------------------
	// // object transformation

	// scale the shape
	glm::mat4 scale = glm::scale(glm::vec3(0.6f, 0.55f, 0.6f));

	// rotate the shape by the alpha, which is incremented in main
	glm::mat4 rotation = glm::rotate(alpha, glm::vec3(1.0f, 0.0f, 0.2f));

	// translate the shape 
	glm::mat4 translation = glm::translate(glm::vec3(2.5f, 0.25f, 1.0f));

	// transform the object
	glm::mat4 model = translation * rotation * scale;

	// ---------------------------------------------------------------

	// set shader
	glUseProgram(gProgramId);

	// retrieve and pass transformation matrices to shader
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));

	// Activate VAO
	glBindVertexArray(pyramid.gMesh.vao);

	// tell the shader that there are not multiple textures
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, false);

	// texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// get the light variables in shader
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPos");
	GLint light1ColorLoc = glGetUniformLocation(gProgramId, "light1Color");
	GLint light1PositionLoc = glGetUniformLocation(gProgramId, "light1Pos");
	GLint light2ColorLoc = glGetUniformLocation(gProgramId, "light2Color");
	GLint light2PositionLoc = glGetUniformLocation(gProgramId, "light2Pos");

	// send light color and light position
	glUniform3f(light1ColorLoc, gLight1Color.r, gLight1Color.g, gLight1Color.b);
	glUniform3f(light1PositionLoc, gLight1Pos.x, gLight1Pos.y, gLight1Pos.z);
	glUniform3f(light2ColorLoc, gLight2Color.r, gLight2Color.g, gLight2Color.b);
	glUniform3f(light2PositionLoc, gLight2Pos.x, gLight2Pos.y, gLight2Pos.z);

	// send object color and camera position
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	const glm::vec3 cameraPosition = cam.position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// draw triangles
	glDrawElements(GL_TRIANGLES, pyramid.gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// overloaded function that takes two textures and binds them both to the pyramid
void render(Pyramid& pyramid, float alpha, Camera& cam, GLuint textureId, GLuint texture2Id)
{
	//########################################################################################################################################
	// Render starts here
	//########################################################################################################################################

	// ---------------------------------------------------------------
	// // object transformation

	// scale the shape
	glm::mat4 scale = glm::scale(glm::vec3(0.5f, 0.5f, 0.5f));

	// rotate the shape by the alpha, which is incremented in main
	glm::mat4 rotation = glm::rotate(alpha, glm::vec3(0.8f, 0.5f, -0.2f));

	// translate the shape 0.5 on the y axis
	glm::mat4 translation = glm::translate(glm::vec3(1.5f, 0.45f, 3.0f));

	// transform the object
	glm::mat4 model = translation * rotation * scale;

	// ---------------------------------------------------------------

	// set shader
	glUseProgram(gProgramId);

	// retrieve and pass transformation matrices to shader
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));

	// tell the shader that there are multiple textures
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, true);

	// Activate VAO
	glBindVertexArray(pyramid.gMesh.vao);

	// texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture2Id);

	// get the light variables in shader
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPos");
	GLint light1ColorLoc = glGetUniformLocation(gProgramId, "light1Color");
	GLint light1PositionLoc = glGetUniformLocation(gProgramId, "light1Pos");
	GLint light2ColorLoc = glGetUniformLocation(gProgramId, "light2Color");
	GLint light2PositionLoc = glGetUniformLocation(gProgramId, "light2Pos");

	// send light color and light position
	glUniform3f(light1ColorLoc, gLight1Color.r, gLight1Color.g, gLight1Color.b);
	glUniform3f(light1PositionLoc, gLight1Pos.x, gLight1Pos.y, gLight1Pos.z);
	glUniform3f(light2ColorLoc, gLight2Color.r, gLight2Color.g, gLight2Color.b);
	glUniform3f(light2PositionLoc, gLight2Pos.x, gLight2Pos.y, gLight2Pos.z);

	// send object color and camera position
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	const glm::vec3 cameraPosition = cam.position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


	// draw triangles
	glDrawElements(GL_TRIANGLES, pyramid.gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// overloaded function that renders a plane object
void render(Plane& plane, float alpha, Camera& cam, GLuint textureId)
{
	//########################################################################################################################################
	// Render starts here
	//########################################################################################################################################


		// ---------------------------------------------------------------
		// // object transformation

		// scale the shape (currently at 1 incase I want to scale it later)
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));

	// rotate the shape by the alpha, which is incremented in main
	glm::mat4 rotation = glm::rotate(alpha, glm::vec3(1.0f, 0.0f, 0.0f));

	// translate the shape 0.5 on the y axis
	glm::mat4 translation = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f));

	// transform the object
	glm::mat4 model = translation * rotation * scale;

	// ---------------------------------------------------------------

	// set shader
	glUseProgram(gProgramId);

	// retrieve and pass transformation matrices to shader
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));

	// Activate VAO
	glBindVertexArray(plane.gMesh.vao);

	// tell the shader that there are not multiple textures
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, false);

	// texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// get the light variables in shader
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPos");
	GLint light1ColorLoc = glGetUniformLocation(gProgramId, "light1Color");
	GLint light1PositionLoc = glGetUniformLocation(gProgramId, "light1Pos");
	GLint light2ColorLoc = glGetUniformLocation(gProgramId, "light2Color");
	GLint light2PositionLoc = glGetUniformLocation(gProgramId, "light2Pos");

	// send light color and light position
	glUniform3f(light1ColorLoc, gLight1Color.r, gLight1Color.g, gLight1Color.b);
	glUniform3f(light1PositionLoc, gLight1Pos.x, gLight1Pos.y, gLight1Pos.z);
	glUniform3f(light2ColorLoc, gLight2Color.r, gLight2Color.g, gLight2Color.b);
	glUniform3f(light2PositionLoc, gLight2Pos.x, gLight2Pos.y, gLight2Pos.z);

	// send object color and camera position
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	const glm::vec3 cameraPosition = cam.position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// draw triangles
	glDrawElements(GL_TRIANGLES, plane.gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

}

// render the frame for a Icosahedron object
// TODO add the abillity to pass values to change the scale, translation, and the directions of rotation
void render(Icosahedron& ico, float alpha, Camera& cam, GLuint textureId)
{
	//########################################################################################################################################
	// Render starts here
	//########################################################################################################################################

		// ---------------------------------------------------------------
		// // object transformation

		// scale the shape
	glm::mat4 scale = glm::scale(glm::vec3(0.28f, 0.28f, 0.28f));

	// rotate the shape by the alpha, which is incremented in main
	glm::mat4 rotation = glm::rotate(alpha, glm::vec3(0.0f, 0.0f, 1.0f));

	// translate the shape 0.5 on the y axis
	glm::mat4 translation = glm::translate(glm::vec3(1.0f, 0.5f, 0.5f));

	// transform the object
	glm::mat4 model = translation * rotation * scale;

	// ---------------------------------------------------------------

	// set shader
	glUseProgram(gProgramId);

	// retrieve and pass transformation matrices to shader
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));

	// Activate VAO
	glBindVertexArray(ico.gMesh.vao);

	// tell the shader that there are not multiple textures
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, false);

	// texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// get the light variables in shader
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPos");
	GLint light1ColorLoc = glGetUniformLocation(gProgramId, "light1Color");
	GLint light1PositionLoc = glGetUniformLocation(gProgramId, "light1Pos");
	GLint light2ColorLoc = glGetUniformLocation(gProgramId, "light2Color");
	GLint light2PositionLoc = glGetUniformLocation(gProgramId, "light2Pos");

	// send light color and light position
	glUniform3f(light1ColorLoc, gLight1Color.r, gLight1Color.g, gLight1Color.b);
	glUniform3f(light1PositionLoc, gLight1Pos.x, gLight1Pos.y, gLight1Pos.z);
	glUniform3f(light2ColorLoc, gLight2Color.r, gLight2Color.g, gLight2Color.b);
	glUniform3f(light2PositionLoc, gLight2Pos.x, gLight2Pos.y, gLight2Pos.z);

	// send object color and camera position
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	const glm::vec3 cameraPosition = cam.position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// draw triangles
	glDrawElements(GL_TRIANGLES, ico.gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// render the frame for a cube object
// TODO add the abillity to pass values to change the scale, translation, and the directions of rotation
void render(Cube& cube, float alpha, Camera& cam, GLuint textureId)
{
	//########################################################################################################################################
	// Render starts here
	//########################################################################################################################################

		// ---------------------------------------------------------------
		// // object transformation

		// scale the shape
	glm::mat4 scale = glm::scale(glm::vec3(3.0f, 0.25f, 5.0f));

	// rotate the shape by the alpha, which is incremented in main
	glm::mat4 rotation = glm::rotate(alpha, glm::vec3(1.0f, 0.0f, 0.0f));

	// translate the shape 0.5 on the y axis
	glm::mat4 translation = glm::translate(glm::vec3(1.5f, 0.1251f, 1.5f));

	// transform the object
	glm::mat4 model = translation * rotation * scale;

	// ---------------------------------------------------------------

	// set shader
	glUseProgram(gProgramId);

	// retrieve and pass transformation matrices to shader
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));

	// Activate VAO
	glBindVertexArray(cube.gMesh.vao);

	// tell the shader that there are not multiple textures
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, false);

	// texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// get the light variables in shader
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPos");
	GLint light1ColorLoc = glGetUniformLocation(gProgramId, "light1Color");
	GLint light1PositionLoc = glGetUniformLocation(gProgramId, "light1Pos");
	GLint light2ColorLoc = glGetUniformLocation(gProgramId, "light2Color");
	GLint light2PositionLoc = glGetUniformLocation(gProgramId, "light2Pos");

	// send light color and light position
	glUniform3f(light1ColorLoc, gLight1Color.r, gLight1Color.g, gLight1Color.b);
	glUniform3f(light1PositionLoc, gLight1Pos.x, gLight1Pos.y, gLight1Pos.z);
	glUniform3f(light2ColorLoc, gLight2Color.r, gLight2Color.g, gLight2Color.b);
	glUniform3f(light2PositionLoc, gLight2Pos.x, gLight2Pos.y, gLight2Pos.z);

	// send object color and camera position
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	const glm::vec3 cameraPosition = cam.position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// draw triangles
	glDrawElements(GL_TRIANGLES, cube.gMesh.nIndices, GL_UNSIGNED_SHORT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}


void render(Sphere& sphere, float alpha, Camera& cam, GLuint textureId)
{
	//########################################################################################################################################
	// Render starts here
	//########################################################################################################################################

	// ---------------------------------------------------------------
	// // object transformation

	// scale the shape
	glm::mat4 scale = glm::scale(glm::vec3(1.0f, 0.9f, 0.9f));

	// rotate the shape by the alpha, which is incremented in main
	glm::mat4 rotation = glm::rotate(alpha, glm::vec3(0.0f, 1.0f, 0.0f));

	// translate the shape 0.5 on the y axis
	glm::mat4 translation = glm::translate(glm::vec3(1.25f, 0.8f, -2.0f));

	// transform the object
	glm::mat4 model = translation * rotation * scale;

	// ---------------------------------------------------------------

	// set shader
	glUseProgram(gProgramId);

	// retrieve and pass transformation matrices to shader
	GLint modelLoc = glGetUniformLocation(gProgramId, "model");
	GLint viewLoc = glGetUniformLocation(gProgramId, "view");
	GLint projLoc = glGetUniformLocation(gProgramId, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjectionMatrix()));

	// tell the shader that there are not multiple textures
	GLuint multipleTexturesLoc = glGetUniformLocation(gProgramId, "multipleTextures");
	glUniform1i(multipleTexturesLoc, false);

	// Activate VAO
	glBindVertexArray(sphere.gMesh.vao);

	// texture binding
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	// get the light variables in shader
	GLint objectColorLoc = glGetUniformLocation(gProgramId, "objectColor");
	GLint viewPositionLoc = glGetUniformLocation(gProgramId, "viewPos");
	GLint light1ColorLoc = glGetUniformLocation(gProgramId, "light1Color");
	GLint light1PositionLoc = glGetUniformLocation(gProgramId, "light1Pos");
	GLint light2ColorLoc = glGetUniformLocation(gProgramId, "light2Color");
	GLint light2PositionLoc = glGetUniformLocation(gProgramId, "light2Pos");

	// send light color and light position
	glUniform3f(light1ColorLoc, gLight1Color.r, gLight1Color.g, gLight1Color.b);
	glUniform3f(light1PositionLoc, gLight1Pos.x, gLight1Pos.y, gLight1Pos.z);
	glUniform3f(light2ColorLoc, gLight2Color.r, gLight2Color.g, gLight2Color.b);
	glUniform3f(light2PositionLoc, gLight2Pos.x, gLight2Pos.y, gLight2Pos.z);

	// send object color and camera position
	glUniform3f(objectColorLoc, gObjectColor.r, gObjectColor.g, gObjectColor.b);
	const glm::vec3 cameraPosition = cam.position;
	glUniform3f(viewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);


	// draw triangles
	glDrawElements(GL_TRIANGLES, sphere.gMesh.nIndices, GL_UNSIGNED_INT, NULL);

	// Deactivate VAO
	glBindVertexArray(0);

	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

// return the programId (needed for camera object???, further testing needed might just make gProgramId public)
GLuint getProgramId()
{
	return gProgramId;
}

bool loadTexture(const char* filename, GLuint& textureId)
{
	int width, height, channels;

	// flips the y for the image so it loads upright
	stbi_set_flip_vertically_on_load(true);

	// load the image with stbi_load
	unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

	if (image)
	{

		//=======================

		// create, activate, and bind texture object
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		// texture wrapping and filtering parameters for each axis
		// we are dealing with 2d images so axis s and t are used
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// generate the texture checking if the image has three channels or 4
		// aka if the image has rgb or rgba
		if (channels == 3) // jpg for example has three channels
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else if (channels == 4) // png for example has four channels
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		// else the image has less than 3 or more than 4 channels print error and return false
		else
		{
			std::cout << "Image does not have 3 or 4 channels it has: " << channels << " channels" << std::endl;
			return false;
		}

		//=======================

		// genreate mip maps
		glGenerateMipmap(GL_TEXTURE_2D);

		// free/unload the image once done
		stbi_image_free(image);
		// unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}
	// did not load image succesfully
	return false;
}

// used to delete the texture object after we are done with it
void deleteTexture(GLuint textureId)
{
	glDeleteTextures(1, &textureId);
}