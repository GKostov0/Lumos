#include "GameWindow.h"
#include "../ErrorCodes.h"

GameWindow::GameWindow()
	: _width(800), _height(600), _bufferWidth(0), _bufferHeight(0), _mainWindow(nullptr)
{
}

GameWindow::GameWindow(GLuint windowWidth, GLuint windowHeight)
	: _width(windowWidth), _height(windowHeight), _bufferWidth(0), _bufferHeight(0), _mainWindow(nullptr)
{
}

GameWindow::~GameWindow()
{
	glfwDestroyWindow(_mainWindow);
	glfwTerminate();
}

int GameWindow::Ititialize()
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

	_mainWindow = glfwCreateWindow(_width, _height, "Lumos", nullptr, nullptr);
	if (!_mainWindow)
	{
		std::cout << "[ERROR]: GLFW window creation Failed!" << std::endl;
		glfwTerminate();

		return ERROR_GLFW_WINDOW_CREATION_FAILED;
	}

	glfwGetFramebufferSize(_mainWindow, &_bufferWidth, &_bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(_mainWindow);

	// Allow modern extentions and features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "[ERROR]: GLEW Init Failed!" << std::endl;
		glfwDestroyWindow(_mainWindow);
		glfwTerminate();

		return ERROR_GLEW_INIT_FAILED;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, _bufferWidth, _bufferHeight);
}
