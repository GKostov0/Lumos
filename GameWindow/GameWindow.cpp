#include "GameWindow.h"
#include "../ErrorCodes.h"

GameWindow::GameWindow()
	: _width(800), _height(600), _bufferWidth(0), _bufferHeight(0), _mainWindow(nullptr),
		_lastX(0), _lastY(0), _xChange(0), _yChange(0), _isInitialMove(true)
{
}

GameWindow::GameWindow(GLuint windowWidth, GLuint windowHeight)
	: _width(windowWidth), _height(windowHeight), _bufferWidth(0), _bufferHeight(0), _mainWindow(nullptr),
		_lastX(0), _lastY(0), _xChange(0), _yChange(0), _isInitialMove(true)
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

	// Handle keyboard and mouse
	CreateCallbacks();

	// Hide the mouse when playing
	glfwSetInputMode(_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glfwSetWindowUserPointer(_mainWindow, this);
}

GLfloat GameWindow::GetXChange()
{
	GLfloat result = _xChange;
	_xChange = 0.0f;

	return result;
}

GLfloat GameWindow::GetYChange()
{
	GLfloat result = _yChange;
	_yChange = 0.0f;

	return result;
}

void GameWindow::CreateCallbacks()
{
	glfwSetKeyCallback(_mainWindow, HandleKeys);
	glfwSetCursorPosCallback(_mainWindow, HandleMouse);
}

// Static
void GameWindow::HandleKeys(GLFWwindow* win, int key, int code, int action, int mode)
{
	GameWindow* pWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(win));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			pWindow->_keys[key] = true;
			//std::cout << "Pressed: " << key << " Key" << std::endl;
		}
		else if (action == GLFW_RELEASE)
		{
			pWindow->_keys[key] = false;
			//std::cout << "Released: " << key << " Key" << std::endl;
		}
	}
}

void GameWindow::HandleMouse(GLFWwindow* win, double xPos, double yPos)
{
	GameWindow* pWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(win));

	if (pWindow->_isInitialMove)
	{
		pWindow->_lastX = xPos;
		pWindow->_lastY = yPos;

		pWindow->_isInitialMove = false;
	}

	// yPos - pWindow->_lastY to invert Y
	pWindow->_xChange = xPos - pWindow->_lastX;
	pWindow->_yChange = pWindow->_lastY - yPos;

	pWindow->_lastX = xPos;
	pWindow->_lastY = yPos;

	//std::cout << "X: " << pWindow->_xChange  << " Y: " << pWindow->_yChange << std::endl;
}
