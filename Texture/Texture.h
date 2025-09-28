#pragma once

#include<iostream>

#include <GL/glew.h>

#include "../STBI/stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* filePath);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint _textureID;
	int _width, _height, _bitDepth;

	const char* _fileLocation;
};