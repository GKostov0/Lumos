#include "Shader.h"

Shader::Shader()
	: _shaderID(0), _uniformProjection(0),
		_uniformModel(0), _uniformView(0)
{

}

Shader::~Shader() 
{
	ClearShader();
}

void Shader::CreateFromString(const char* vCode, const char* fCode)
{
	CompileShader(vCode, fCode);
}

void Shader::CreateFromFiles(const char* vLocation, const char* fLocation)
{
	CompileShader(ReadFile(vLocation).c_str(), ReadFile(fLocation).c_str());
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "[Error]: Could NOT open file - " << fileLocation << std::endl;
		return "";
	}

	std::string line = "";

	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::UseShader()
{
	if (_shaderID != 0)
	{
		glUseProgram(_shaderID);
	}
	else
	{
		std::cout << "[ERROR]: Can't use program " << _shaderID << std::endl;
		return;
	}
}

void Shader::ClearShader()
{
	if (_shaderID != 0)
	{
		glDeleteProgram(_shaderID);
		_shaderID = 0;
	}

	_uniformModel = 0;
	_uniformProjection = 0;
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar log[1024] = { 0 };

	// Compilation
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(log), NULL, log);

		std::cout << "[ERROR]: Shader program " << shaderType << " compilation Failed!\n[INFO]: " << log << std::endl;
		return;
	}

	glAttachShader(_shaderID, shader);
}

void Shader::CompileShader(const char* vCode, const char* fCode)
{
	_shaderID = glCreateProgram();

	if (!_shaderID)
	{
		std::cout << "[ERROR]: Shader program Failed!" << std::endl;
		return;
	}

	AddShader(_shaderID, vCode, GL_VERTEX_SHADER);
	AddShader(_shaderID, fCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar log[1024] = { 0 };

	// Linking
	glLinkProgram(_shaderID);
	glGetProgramiv(_shaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(_shaderID, sizeof(log), NULL, log);

		std::cout << "[ERROR]: Shader program linking Failed! \n[INFO]: " << log << std::endl;
		return;
	}

	// Validation
	glValidateProgram(_shaderID);
	glGetProgramiv(_shaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(_shaderID, sizeof(log), NULL, log);

		std::cout << "[ERROR]: Shader program validation Failed! \n[INFO]: " << log << std::endl;
		return;
	}

	_uniformModel = glGetUniformLocation(_shaderID, "model");
	_uniformView = glGetUniformLocation(_shaderID, "view");
	_uniformProjection = glGetUniformLocation(_shaderID, "projection");
	_uniformAmbientColor = glGetUniformLocation(_shaderID, "directionalLight.color");
	_uniformAmbientIntensity = glGetUniformLocation(_shaderID, "directionalLight.ambientIntensity");
}
