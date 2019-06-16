#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../headers.h"

enum Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldup;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat speed;
	GLfloat sensitivity;
	GLfloat zoom;

	Camera(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 up_ = glm::vec3(0, 1, 0), GLfloat ya = -90.0f, GLfloat pit = -10.0f);
	glm::mat4 getViewMat();
	void keyboardHandler(Movement, GLfloat dtime);
	void mouseMoveHandler(GLfloat, GLfloat, GLboolean constrainPitch = true);
	void mouseScrollHandler(GLfloat y);
	void updateCamera();
};

#endif
