#include "Material.h"

Material::Material()
	: _specularIntensity{}, _shininess{}
{
}

Material::Material(GLfloat shine, GLfloat sIntensity)
	: _specularIntensity{ sIntensity }, _shininess{ shine }
{
}

Material::~Material()
{
}

void Material::UseMaterial(GLuint shininessLocation, GLuint specularIntensityLocation)
{
	glUniform1f(shininessLocation, _shininess);
	glUniform1f(specularIntensityLocation, _specularIntensity);
}