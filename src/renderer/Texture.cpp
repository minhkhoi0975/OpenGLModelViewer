#include "Texture.h"
#include "Image.h"

#include <iostream>

Texture::~Texture()
{
	Shutdown();
}

void Texture::Init()
{
	if (id != 0)
		return;

	glGenTextures(1, &id);
}

void Texture::Shutdown()
{
	if (id == 0)
		return;

	glDeleteTextures(1, &id);
	id = 0;
}

void Texture::Use()
{
	if (id == 0)
		return;

	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Unuse()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::LoadImage(Image* image, TextureType textureType)
{
	if (id == 0 || image == nullptr || image->GetData() == nullptr)
		return;

	GLenum imageFormat = GL_RED;
	int channelCount = image->GetChannelCount();
	if (channelCount == 1)
		imageFormat = GL_RED;
	else if (channelCount == 2)
		imageFormat = GL_RG;
	else if (channelCount == 3)
		imageFormat = GL_RGB;
	else if (channelCount == 4)
		imageFormat = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, image->GetWidth(), image->GetHeight(), 0, imageFormat, GL_UNSIGNED_BYTE, image->GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->filePath = image->GetFilePath();
	this->textureType = textureType;
}
