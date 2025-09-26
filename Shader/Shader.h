#pragma once

#include <iostream>
#include <string>
#include <fstream>

#include<GL/glew.h>

class Shader
{
public:
	Shader();
	~Shader();

	void CreateFromString(const char* vCode, const char* fCode);
	void CreateFromFiles(const char* vLocation, const char* fLocation);

	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation() const { return _uniformProjection; }
	GLuint GetModelLocation() const { return _uniformModel; };

	void UseShader();
	void ClearShader();

private:
	GLuint _shaderID, _uniformProjection, _uniformModel;

	void AddShader(GLuint program, const char* shaderCode, GLenum shaderType);
	void CompileShader(const char* vCode, const char* fCode);
};