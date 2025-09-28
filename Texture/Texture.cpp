#include "Texture.h"

Texture::Texture()
	: _textureID(0), _width(0), _height(0), _bitDepth(0), _fileLocation("")
{
}

Texture::Texture(const char* filePath)
	: _textureID(0), _width(0), _height(0), _bitDepth(0), _fileLocation(filePath)
{
}

Texture::~Texture()
{
	ClearTexture();
}

void Texture::LoadTexture()
{
	unsigned char* texData = stbi_load(_fileLocation, &_width, &_height, &_bitDepth, 0);
	if (!texData)
	{
		std::cout << "[Error]: Failed to load " << _fileLocation << std::endl;
		return;
	}

	glGenTextures(1, &_textureID);
	glBindTexture(GL_TEXTURE_2D, _textureID);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(texData);
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &_textureID);
	_textureID = 0;
	_width = 0;
	_height = 0;
	_bitDepth = 0;
	_fileLocation = "";
}
