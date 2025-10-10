#include "Light.h"

Light::Light()
	: _color{1.0f}, _ambientIntensity{0.5f}
{
}

Light::Light(glm::vec3 lightColor, GLfloat ambientIntensity)
	: _color{ lightColor }, _ambientIntensity{ ambientIntensity }
{
}

Light::~Light()
{
}

void Light::UseLight(GLfloat ambientColorLocation, GLfloat ambientIntensityLocation)
{
	glUniform3f(ambientColorLocation, _color.x, _color.y, _color.z);
	glUniform1f(ambientIntensityLocation, _ambientIntensity);
}
