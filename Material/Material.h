#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat shine, GLfloat sIntensity);
	~Material();

	void UseMaterial(GLuint shininessLocation, GLuint specularIntensityLocation);

private:
	GLfloat _shininess;
	GLfloat _specularIntensity;
};