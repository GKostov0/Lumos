#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GameWindow
{
public:
	GameWindow();
	GameWindow(GLuint windowWidth, GLuint windowHeight);
	~GameWindow();

	int Ititialize();

	GLfloat GetBufferWidth() const { return _bufferWidth; }
	GLfloat GetBufferHeight() const { return _bufferHeight; }

	bool GetShouldClose() { return glfwWindowShouldClose(_mainWindow); }

	bool* GetKeys() { return _keys; }

	GLfloat GetXChange();
	GLfloat GetYChange();

	void SwapBuffers() { glfwSwapBuffers(_mainWindow); }

private:
	GLFWwindow* _mainWindow;
	GLint _width, _height;
	GLint _bufferWidth, _bufferHeight;

	bool _keys[1024] = {};

	GLfloat _lastX, _lastY;
	GLfloat _xChange, _yChange;
	bool _isInitialMove;

	void CreateCallbacks();

	static void HandleKeys(GLFWwindow* win, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* win, double xPos, double yPos);
};