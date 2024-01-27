#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <filesystem>

Image::Image(const char* filePath, bool flipVertically)
{
	Load(filePath, flipVertically);
}

Image::~Image()
{
	Unload();
}

bool Image::Load(const char* filePath, bool flipVertically)
{
	if (data)
		return false;

	// The origin of the image is in top left, but the origin of UV is bottom left.
	// Without calling this function, the image is vertically reversed.
	stbi_set_flip_vertically_on_load(flipVertically);

	data = stbi_load(filePath, &width, &height, &channelCount, 0);
	if (!data)
	{
		std::cout << "Cannot load image from file path: " << filePath << std::endl << std::endl;  
		stbi_image_free(data);
		return false;
	}

	this->filePath = std::filesystem::absolute(filePath);
	return true;
}

void Image::Unload()
{
	if (data == nullptr)
		return;

	stbi_image_free(data);

	width = 0;
	height = 0;
	channelCount = 0;
	data = nullptr;
}