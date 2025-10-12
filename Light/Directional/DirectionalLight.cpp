#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
	: Light{}, _direction{ 0.0f, -1.0f, 0.0f }
{
}

DirectionalLight::DirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity)
	: Light{ lightColor , ambientIntensity }, _direction{ 0.0f, -1.0f, 0.0f }
{
}

DirectionalLight::DirectionalLight(glm::vec3 lightColor, GLfloat ambientIntensity, glm::vec3 lightDirection, GLfloat diffuseIntensity)
	: Light{ lightColor , ambientIntensity, diffuseIntensity }, _direction{ lightDirection }
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::UseLight(GLuint ambientColorLocation, GLuint ambientIntensityLocation,
	GLuint diffuseDirectionLocation, GLuint diffuseIntensityLocation)
{
	// Ambient
	glUniform3f(ambientColorLocation, _color.x, _color.y, _color.z);
	glUniform1f(ambientIntensityLocation, _ambientIntensity);

	// Diffuse
	glUniform3f(diffuseDirectionLocation, _direction.x, _direction.y, _direction.z);
	glUniform1f(diffuseIntensityLocation, _diffuseIntensity);
}