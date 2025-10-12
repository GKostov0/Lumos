#pragma once

#include <GL/glew.h>
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(glm::vec3 lightColor, GLfloat ambientIntensity);
	Light(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 lightDirection, GLfloat diffuseIntensity);
	~Light();

	void UseLight(GLuint ambientColorLocation, GLuint ambientIntensityLocation,
		GLuint diffuseDirectionLocation, GLuint diffuseIntensityLocation);

private:

	// Ambient
	glm::vec3 _color;
	GLfloat _ambientIntensity;


	// Diffuse
	glm::vec3 _direction;
	GLfloat _diffuseIntensity;
};