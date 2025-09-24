#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

// Window dimentions
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{

	// Init GLFW
	if (!glfwInit())
	{
		std::cout << "[ERROR]: GLFW Init Failed!" << std::endl;
		glfwTerminate();

		// TODO: Add Error Codes
		return -1;
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

		// TODO: Add Error Codes
		return -1;
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

		// TODO: Add Error Codes
		return -1;
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