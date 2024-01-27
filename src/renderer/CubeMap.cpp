#include "CubeMap.h"

#include <glad/glad.h>

CubeMap::~CubeMap()
{
	Shutdown();
}

void CubeMap::Init()
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void CubeMap::Shutdown()
{
	if (id == 0)
		return;

	ClearTextures();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::Use()
{
	if (id == 0)
		return;

	glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::Unuse()
{
	if (id == 0)
		return;

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::LoadImages(const char* filePaths[6])
{
	ClearTextures();

	images.resize(6);

	for (unsigned int i = 0; i < 6; ++i)
	{
		std::shared_ptr<Image> image = std::make_shared<Image>(filePaths[i]);
		if (image->GetData() == nullptr)
		{	
			images[i] = nullptr;
			continue;
		}

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

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, imageFormat, image->GetWidth(), image->GetHeight(), 0, imageFormat, GL_UNSIGNED_BYTE, image->GetData());
		images[i] = image;
	}
}

void CubeMap::ClearTextures()
{
	for (unsigned int i = 0; i < images.size(); ++i)
	{
		if (images[i] == nullptr)
			continue;

		images[i]->Unload();
	}

	images.clear();
}
