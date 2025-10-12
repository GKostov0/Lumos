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
#include "Texture/Texture.h"
#include "Light/Directional/DirectionalLight.h"
#include "Material/Material.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STBI/stb_image.h"

const float toRadians = 3.14159265f / 180.0f;

GameWindow gameWindow(1366, 768);

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material mattMaterial;

DirectionalLight mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vShader = "Shader/shader.vert";
static const char* fShader = "Shader/shader.frag";

void CalculateAverageNormals(unsigned int* indices, unsigned int indiceCount,
	GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);

		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	//unsigned int indices[] =
	//{
	//	0, 1, 2, // Front
	//	0, 1, 3, // Left Side
	//	2, 1, 4, // Right Side
	//	3, 1, 4, // Back Side
	//	0, 4, 3, // Bottom Left Side
	//	0, 4, 2  // Bottom Right Side
	//};

	//GLfloat vertices[] =
	//{
	//	// Positions			 //Texture       // Normals
	////   x,      y,     z        u        v
	//	-1.0f,  -1.0f,  1.0f,    0.0f,    0.0f,	  0.0f, 0.0f, 0.0f,			// Bottom Left Front
	//	 0.0f,   1.0f,  2.0f,	 0.5f,    1.0f,   0.0f, 0.0f, 0.0f,			// Top
	//	 1.0f,  -1.0f,  1.0f,	 1.0f,	  0.0f,	  0.0f, 0.0f, 0.0f,			// Bottom Right Front
	//	-1.0f,  -1.0f,  3.0f,	 0.0f,    1.0f,	  0.0f, 0.0f, 0.0f,			// Bottom Left Back
	//	 1.0f,  -1.0f,  3.0f,	 1.0f,    1.0f,   0.0f, 0.0f, 0.0f			// Bottom Right Back
	//};

	unsigned int indices[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	CalculateAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	Mesh* obj2 = new Mesh();

	obj1->CreateMesh(vertices, indices, 32, 12);
	obj2->CreateMesh(vertices, indices, 32, 12);

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
	gameWindow.Initialize();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3{}, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Images/brick.png");
	dirtTexture = Texture("Images/dirt.png");

	brickTexture.LoadTexture();
	dirtTexture.LoadTexture();

	shinyMaterial = Material(128.0f, 1.0f);
	mattMaterial = Material(8.0f, 0.6f);

	mainLight = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.5f, glm::vec3(0.0f, -1.0f, 0.0f), 0.3f);

	GLuint uniformModel = 0, uniformProjection = 0, unifromView = 0, uniformEyePosition = 0;
	GLuint uniformAmbientColor = 0, uniformAmbientIntensity = 0;
	GLuint uniformDiffuseDirection = 0, uniformDiffuseIntensity = 0;
	GLuint uniformSpecularShine = 0, uniformSpecularIntensity = 0;

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
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		// Ambient
		uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();

		// Diffuse
		uniformDiffuseDirection = shaderList[0].GetDiffuseDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();

		// Material
		// Specular
		uniformSpecularShine = shaderList[0].GetSpecularShininessLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();

		mainLight.UseLight(uniformAmbientColor, uniformAmbientIntensity,
							uniformDiffuseDirection, uniformDiffuseIntensity);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(unifromView, 1, GL_FALSE, glm::value_ptr(camera.CalculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetCameraPosition().x, camera.GetCameraPosition().y, camera.GetCameraPosition().z);

		// Identity matrix
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		
		// Moves the triangle
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularShine, uniformSpecularIntensity);
		meshList[0]->RenderMesh();

		// Identity matrix
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		dirtTexture.UseTexture();
		mattMaterial.UseMaterial(uniformSpecularShine, uniformSpecularIntensity);
		meshList[1]->RenderMesh();

		glUseProgram(0);

		// Double buffer swap
		gameWindow.SwapBuffers();
	}
}