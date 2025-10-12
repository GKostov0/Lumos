#include "Light.h"

Light::Light()
	: _color{1.0f}, _ambientIntensity{0.5f}, _diffuseIntensity{0.5f}
{
}

Light::Light(glm::vec3 lightColor, GLfloat ambientIntensity)
	: _color{ lightColor }, _ambientIntensity{ ambientIntensity }, _diffuseIntensity{ 0.5f }
{
}

Light::Light(glm::vec3 lightColor, GLfloat ambientIntensity, GLfloat diffuseIntensity)
	: _color{ lightColor }, _ambientIntensity{ ambientIntensity }, _diffuseIntensity{ diffuseIntensity }
{
}

Light::~Light()
{
}
