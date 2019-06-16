#include "Camera.h"
#include <iostream>

using namespace std;

Camera::Camera(glm::vec3 pos, glm::vec3 up_, GLfloat ya, GLfloat pit) {
	front = glm::vec3(0.0f, 0.0f, 0.0f);
	speed = 9.0f;
	sensitivity = 0.25f;
	zoom = 45.0f;

	position = pos;
	worldup = up_;
	yaw = ya;
	pitch = pit;

	updateCamera();
}

glm::mat4 Camera::getViewMat() {
	return glm::lookAt(position, position + front, up);
}

void Camera::keyboardHandler(Movement dir, GLfloat dtime) {
	GLfloat vel = speed * dtime;
	switch (dir) {
	case FORWARD: position += front * vel; break;
	case BACKWARD: position -= front * vel; break;
	case LEFT: position -= right * vel; break;
	case RIGHT: position += right * vel; break;
	}
}

void Camera::mouseMoveHandler(GLfloat x, GLfloat y, GLboolean constrainPitch) {
	//cout << y << " " << pitch << " ";
	x *= sensitivity;
	y *= sensitivity;
	
	yaw += x;
	pitch += y;

	//cout << y << " " <<  pitch << endl;
	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCamera();
}

void Camera::mouseScrollHandler(GLfloat y) {
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= y;
	}
	if (zoom <= 1.0f) {
		zoom = 1.0f;
	}
	if (zoom >= 45.0f) {
		zoom = 45.0f;
	}
}

void Camera::updateCamera() {
	glm::vec3 f;
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(f);
	//cout << front.x << " " << front.y << " " << front.z << " " << pitch << endl;
	right = glm::normalize(glm::cross(front, worldup));
	up = glm::normalize(glm::cross(right, front));
}