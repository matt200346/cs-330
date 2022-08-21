// Matthew Eaton
// CS 330 3D scene final project
// 08/14/22
// this is the header for the camera object

#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>
class Camera {
private:
	// default starting z position
	const float DEFAULT_ZPOS = 6.0f;
	double mouseSensitivity;
	double yaw;
	double pitch;
	glm::mat4 view;
	glm::mat4 projection;
	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;
	bool orthoCheck;

	void updateCameraVectors();


public:
	Camera(int windowHeight, int windowWidth);
	void updateCamera();
	void translateCamera(float velocity, char direction);
	void mouseMovement(double xoffset, double yoffset);
	void setOrthoCheck();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	// camera vectors
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 worldUp;
	glm::vec3 right;
};


#endif