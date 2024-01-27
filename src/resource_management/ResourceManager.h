#pragma once

#include <filesystem>
#include <map>

#include "../renderer/Shader.h"
#include "../renderer/Image.h"
#include "../renderer/Texture.h"
#include "../renderer/Model.h"

class ResourceManager
{
public:
	static ResourceManager* GetInstance() {return instance;}

	~ResourceManager();

	void Init();
	void Shutdown();

	std::weak_ptr<Shader> LoadShader(std::string shaderName, const char* vertexShaderPath, const char* fragmentShaderPath);
	std::weak_ptr<Shader> GetShader(std::string shaderName);

	std::weak_ptr<Texture> GetTexture(const char* path, TextureType textureType);
	std::weak_ptr<Model> GetModel(const char* path);

private:
	static ResourceManager* instance;

	std::map<std::string, std::shared_ptr<Shader>> shaders;
	std::map<std::filesystem::path, std::shared_ptr<Texture>> textures;
	std::map<std::filesystem::path, std::shared_ptr<Model>> models;
};