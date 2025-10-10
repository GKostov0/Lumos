#pragma once

#include <GL/glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 lightColor, GLfloat ambientIntensity);
	~Light();

	void UseLight(GLfloat ambientColorLocation, GLfloat ambientIntensityLocation);

private:
	glm::vec3 _color;
	GLfloat _ambientIntensity;
};