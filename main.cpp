#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;

#define ERROR_GLFW_INIT_FAILED 1
#define ERROR_GLFW_WINDOW_CREATION_FAILED 2
#define ERROR_GLEW_INIT_FAILED 3

GLuint VAO, VBO, shader;

// Vertex Shader
static const char* vShader = "			\n\
#version 330							\n\
										\n\
layout (location = 0) in vec3 pos;		\n\
										\n\
void main()								\n\
{										\n\
	gl_Position = vec4(pos, 1.0f);		\n\
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

	// Main loop
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Handle user input
		glfwPollEvents();

		// Clear window
		glClearColor(1.0f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Double buffer swap
		glfwSwapBuffers(mainWindow);
	}
}