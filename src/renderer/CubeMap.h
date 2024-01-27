#pragma once

#include <vector>
#include <memory>

#include "Image.h"

class CubeMap
{
public:
	~CubeMap();

	void Init();
	void Shutdown();

	void Use();
	void Unuse();

	void LoadImages(const char* filePaths[6]);
	void ClearTextures();

private:
	unsigned int id = 0;
	std::vector<std::shared_ptr<Image>> images;
};