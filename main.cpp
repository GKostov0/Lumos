#include <iostream>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ErrorCodes.h"
#include "Mesh/Mesh.h"
#include "Shader/Shader.h"
#include "GameWindow/GameWindow.h"
#include "Camera/Camera.h"

const float toRadians = 3.14159265f / 180.0f;

GameWindow gameWindow(800, 600);

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shader/shader.vert";
static const char* fShader = "Shader/shader.frag";

void CreateObjects()
{
	unsigned int indices[] =
	{
		0, 3, 1, // Left side
		1, 3, 2, // Right side
		2, 3, 0, // Back side
		0, 1, 2  // Bottom side
	};

	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* obj1 = new Mesh();
	Mesh* obj2 = new Mesh();

	obj1->CreateMesh(vertices, indices, 12, 12);
	obj2->CreateMesh(vertices, indices, 12, 12);

	meshList.push_back(obj1);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	gameWindow.Ititialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformModel, uniformProjection, unifromView;

	glm::mat4 projection = glm::perspective(45.0f, gameWindow.GetBufferWidth()/ gameWindow.GetBufferHeight(), 0.1f, 100.0f);

	// Main loop
	while (!gameWindow.GetShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Handle user input
		glfwPollEvents();

		camera.KeyControls(gameWindow.GetKeys(), deltaTime);
		camera.MouseControl(gameWindow.GetXChange(), gameWindow.GetYChange());

		// Clear window
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		unifromView = shaderList[0].GetViewLocation();

		// Identity matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		
		// Moves the triangle
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(unifromView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));

		meshList[0]->RenderMesh();

		// Identity matrix
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, -3.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		glUseProgram(0);

		// Double buffer swap
		gameWindow.SwapBuffers();
	}
}