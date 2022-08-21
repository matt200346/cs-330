// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this file contains the definitions and logic for the camera object

#include <iostream>         // cout, cerr etc.
#include <cstdlib>          // EXIT_FAILURE
#include <GL/glew.h>        // GLEW library
#include <GLFW/glfw3.h>     // GLFW library

// GLM Math Header inclusions
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.h"



// default constructor
Camera::Camera(int windowHeight, int windowWidth)
{
	// set initial values for variables
	this->mouseSensitivity = 0.1f;
	this->yaw = -110.0f;
	this->pitch = -20.0f;
	this->WINDOW_HEIGHT = windowHeight;
	this->WINDOW_WIDTH = windowWidth;
	orthoCheck = false;

	// camera vectors
	this->position = glm::vec3(2.7f, 2.5f, DEFAULT_ZPOS);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// uypdate the camera vectors
	updateCameraVectors();

	// set the initial matricies view and projection
	this->view = glm::lookAt(position, (position + front), up);
	this->projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);

}

// recalculates the projection and view matricies
void Camera::updateCamera()
{
	
	
	if (!orthoCheck)
	{
		// calculate view matrix
		view = glm::lookAt(position, (position + front), up);

		// calculate projection matrix
		projection = glm::perspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGHT, 0.1f, 100.0f);
	}
	else
	{
		GLfloat orthoWidth = (GLfloat)WINDOW_WIDTH * 0.005f;
		GLfloat orthoHeight = (GLfloat)WINDOW_HEIGHT * 0.005f;

		// calculate projection matrix
		projection = glm::ortho(orthoWidth, -(orthoWidth), orthoHeight, -(orthoHeight), 0.1f, 100.0f);

		// calculate view matrix
		view = glm::lookAt(position, (position + front), -(up));
	}
}

// translate the camera based on the passed value and direction passed to it
// the first argument specifies speed * deltatime and the second argument
// specifies the character that was passed to determine direction
void Camera::translateCamera(float velocity, char direction)
{
	// set new positions based on key that was hit
	switch (direction)
	{
		case ('w'):
		{
			position += front * velocity;
		}
		break;

		case ('s'):
		{
			position -= front * velocity;
		}
		break;

		case('a'):
		{
			position -= right * velocity;
		}
		break;

		case('d'):
		{
			position += right * velocity;
		}
		break;

		case('q'):
		{
			position += up * velocity;
		}
		break;

		case('e'):
		{
			position -= up * velocity;
		}
		break;
	}

}

// move the camera based on mouse movement
void Camera::mouseMovement(double xoffset, double yoffset)
{
	// multiply the offset by the mouse sensitivity
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	// add the offset to the current yaw and pitch
	yaw += xoffset;
	pitch += yoffset;

	// make sure that the camera will not flip if the user looks past directly up or down
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	// update camera vectors
	updateCameraVectors();
}

// calculate and update the camera vectors
void Camera::updateCameraVectors()
{
	// calculate the camera objects front vector
	glm::vec3 localFront;
	localFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	localFront.y = sin(glm::radians(pitch));
	localFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(localFront);

	// calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::setOrthoCheck()
{
	orthoCheck = !orthoCheck;
}

// returns the view matrix
glm::mat4 Camera::getViewMatrix()
{
	return view;
}

// returns the projection matrix
glm::mat4 Camera::getProjectionMatrix()
{
	return projection;
}