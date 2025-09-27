#include "Camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp,
				GLfloat startYaw, GLfloat startPitch,
				GLfloat startMoveSpeed, GLfloat startTurnSpeed)
	:
	_position(startPosition), _front(glm::vec3(0.0f, 0.0f, -1.0f)),
	_up(startUp), _yaw(startYaw), _pitch(startPitch),
	_moveSpeed(startMoveSpeed), _turnSpeed(startTurnSpeed),
	_right(glm::vec3()), _worldUp(glm::vec3())
{
	Update();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front.y = sin(glm::radians(_pitch));
	_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(_front);

	_right = glm::normalize(glm::cross(_front, _worldUp));
	_up = glm::normalize(glm::cross(_right, _front));
}

void Camera::KeyControls(bool* keys)
{
	if (keys[GLFW_KEY_W])
	{
		_position += _front * _moveSpeed;
	}

	if (keys[GLFW_KEY_S])
	{
		_position -= _front * _moveSpeed;
	}

	if (keys[GLFW_KEY_A])
	{
		_position -= _right * _moveSpeed;
	}

	if (keys[GLFW_KEY_D])
	{
		_position += _right * _moveSpeed;
	}
}