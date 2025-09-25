#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <mat4x4.hpp>

#include "ErrorCodes.h"

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shaderProgram, uniformXMove;

bool isGoingLeft = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.9f;
float speed = 0.0005f;

// Vertex Shader
static const char* vShader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
uniform float xMove;														\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = vec4(0.4f * pos.x + xMove, 0.4f * pos.y, pos.z, 1.0f);	\n\
}";

// Fragment Shader
static const char* fShader = "				\n\
#version 330								\n\
											\n\
out vec4 color;								\n\
											\n\
void main()									\n\
{											\n\
	color = vec4(0.3f, 0.4f, 1.0f, 1.0f);	\n\
}";

int AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
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
		return ERROR_SHADER_COMPILATION_FAILED;
	}

	glAttachShader(shaderProgram, shader);

	return NO_ERROR;
}

int CompileShaders()
{
	shaderProgram = glCreateProgram();

	if (!shaderProgram)
	{
		std::cout << "[ERROR]: Shader program Failed!" << std::endl;

		return ERROR_SHADER_PROGRAM_FAILED;
	}

	if (AddShader(shaderProgram, vShader, GL_VERTEX_SHADER) != NO_ERROR)
	{
		return ERROR_SHADER_COMPILATION_FAILED;
	}
	if (AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER) != NO_ERROR)
	{
		return ERROR_SHADER_COMPILATION_FAILED;
	}

	GLint result = 0;
	GLchar log[1024] = { 0 };

	// Linking
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);

		std::cout << "[ERROR]: Shader program linking Failed! \n[INFO]: " << log << std::endl;
		return ERROR_SHADER_LINK_FAILED;
	}

	// Validation
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(log), NULL, log);

		std::cout << "[ERROR]: Shader program validation Failed! \n[INFO]: " << log << std::endl;
		return ERROR_SHADER_VALIDATION_FAILED;
	}

	uniformXMove = glGetUniformLocation(shaderProgram, "xMove");

	return NO_ERROR;
}

void CreateTriangle()
{
	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

int main()
{

	// Init GLFW
	if (!glfwInit())
	{
		std::cout << "[ERROR]: GLFW Init Failed!" << std::endl;
		glfwTerminate();

		return ERROR_GLFW_INIT_FAILED;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Coe profile mans no backwards comaptibility and no old or depricated stuff!
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Lumos", nullptr, nullptr);
	if (!mainWindow)
	{
		std::cout << "[ERROR]: GLFW window creation Failed!" << std::endl;
		glfwTerminate();

		return ERROR_GLFW_WINDOW_CREATION_FAILED;
	}

	// Get Buffer size info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extentions and features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "[ERROR]: GLEW Init Failed!" << std::endl;
		glfwDestroyWindow(mainWindow);
		glfwTerminate();

		return ERROR_GLEW_INIT_FAILED;
	}

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	// Main loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Handle user input
		glfwPollEvents();

		// Move triangle
		if (isGoingLeft)
		{
			triangleOffset -= speed;
		}
		else
		{
			triangleOffset += speed;
		}

		if (abs(triangleOffset) >= triangleMaxOffset)
		{
			isGoingLeft = !isGoingLeft;
		}

		// Clear window
		glClearColor(1.0f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		// Moves the triangle
		glUniform1f(uniformXMove, triangleOffset);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		// Double buffer swap
		glfwSwapBuffers(mainWindow);
	}
}