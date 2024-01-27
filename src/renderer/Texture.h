#pragma once
#include <glad/glad.h>
#include <filesystem>

enum class TextureType
{
	Unknown = 0,
	Diffuse = 1,
	Specular = 2,
	Normal = 3
};

class Texture
{
public:
	~Texture();

	void Init();
	void Shutdown();

	void Use();
	void Unuse();

	void LoadImage(class Image* image, TextureType textureType = TextureType::Unknown);

	std::filesystem::path GetFilePath() const {return filePath;}
	TextureType GetTextureType() const {return textureType;}

private:
	unsigned int id = 0;
	std::filesystem::path filePath;
	TextureType textureType = TextureType::Unknown;
};