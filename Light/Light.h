#pragma once

#include <GL/glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 lightColor, GLfloat ambientIntensity);
	Light(glm::vec3 lightColor, GLfloat ambientIntensity, GLfloat diffuseIntensity);

	virtual ~Light();

protected:

	// Ambient
	glm::vec3 _color;
	GLfloat _ambientIntensity;


	// Diffuse
	GLfloat _diffuseIntensity;
};