#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ErrorCodes.h"

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shaderProgram;
GLuint uniformModel, uniformProjection;

bool isGoingLeft = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.6f;
float speed = 0.0002f;

float currAngle = 0.0f;

// Vertex Shader
static const char* vShader = "												\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
out vec4 vColor;															\n\
																			\n\
uniform mat4 model;															\n\
uniform mat4 projection;													\n\
																			\n\
void main()																	\n\
{																			\n\
	gl_Position = projection * model * vec4(pos, 1.0f);						\n\
	vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);							\n\
}";

// Fragment Shader
static const char* fShader = "				\n\
#version 330								\n\
											\n\
in vec4 vColor;								\n\
											\n\
out vec4 color;								\n\
											\n\
void main()									\n\
{											\n\
	color = vColor;							\n\
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

	uniformModel = glGetUniformLocation(shaderProgram, "model");
	uniformProjection = glGetUniformLocation(shaderProgram, "projection");

	return NO_ERROR;
}

void CreateTriangle()
{
	unsigned int indices[] =
	{
		0, 3, 1, // Left side
		1, 3, 2, // Right side
		2, 3, 0, // Front side
		0, 1, 2  // Bottom side
	};

	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind this after unbinding the VAO
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

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

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

		currAngle = currAngle >= 360.0f ? 0.0f : currAngle + 0.04f;

		if (abs(triangleOffset) >= triangleMaxOffset)
		{
			isGoingLeft = !isGoingLeft;
		}

		// Clear window
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);

		// Identity matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(triangleOffset, 0.0f, -3.0f));
		//model = glm::rotate(model, currAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		
		// Moves the triangle
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glUseProgram(0);

		// Double buffer swap
		glfwSwapBuffers(mainWindow);
	}
}