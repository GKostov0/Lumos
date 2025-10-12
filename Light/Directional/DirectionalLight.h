#pragma once

#include "../Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity);
	DirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 lightDirection, GLfloat diffuseIntensity);

	~DirectionalLight();

	void UseLight(GLuint ambientColorLocation, GLuint ambientIntensityLocation,
		GLuint diffuseDirectionLocation, GLuint diffuseIntensityLocation);

private:
	glm::vec3 _direction;
};