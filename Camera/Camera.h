#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(glm::vec3 startPosition, glm::vec3 startUp,
		GLfloat startYaw, GLfloat startPitch,
		GLfloat startMoveSpeed, GLfloat startTurnSpeed);

	void KeyControls(bool* keys);

	~Camera();

private:

	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _worldUp;

	GLfloat _yaw;
	GLfloat _pitch;

	GLfloat _moveSpeed;
	GLfloat _turnSpeed;

	void Update();
};

