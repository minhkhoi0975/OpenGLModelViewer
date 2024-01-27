#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::~ResourceManager()
{
	Shutdown();
}

void ResourceManager::Init()
{
	if (instance == nullptr)
		instance = this;
}

void ResourceManager::Shutdown()
{
	// Unload all models.
	for (auto itr : models)
	{
		if (itr.second == nullptr)
			continue;

		itr.second->ClearModel();
	}

	// Unload all textures.
	for (auto itr : textures)
	{
		if (itr.second == nullptr)
			continue;

		itr.second->Shutdown();
	}

	// Unload all shaders.
	for (auto itr : shaders)
	{
		if (itr.second == nullptr)
			continue;

		itr.second->Shutdown();
	}

	if (instance == this)
		instance = nullptr;
}

std::weak_ptr<Shader> ResourceManager::LoadShader(std::string shaderName, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	// Check if the shader already exists.
	auto shader = shaders.find(shaderName);
	if (shaders.find(shaderName) != shaders.end())
		return shader->second;

	std::shared_ptr<Shader> newShader = std::make_shared<Shader>();
	newShader->Init(vertexShaderPath, fragmentShaderPath);
	shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(shaderName, newShader));

	return newShader;
}

std::weak_ptr<Shader> ResourceManager::GetShader(std::string shaderName)
{
	auto shader = shaders.find(shaderName);
	if (shader == shaders.end())
		return std::weak_ptr<Shader>();

	return shader->second;
}

std::weak_ptr<Texture> ResourceManager::GetTexture(const char* path, TextureType textureType)
{
	std::filesystem::path absolutePath = std::filesystem::absolute(path);

	// If the texture is already loaded, grab that texture.
	auto loadedTexture = textures.find(absolutePath);
	if (loadedTexture != textures.end())
		return loadedTexture->second;

	// Create a new texture.
	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
	newTexture->Init();
	newTexture->Use();
	Image image(path, true);
	newTexture->LoadImage(&image, textureType);
	textures.insert(std::pair<std::filesystem::path, std::shared_ptr<Texture>>(absolutePath, newTexture));

	return newTexture;
}

std::weak_ptr<Model> ResourceManager::GetModel(const char* path)
{
	std::filesystem::path absolutePath = std::filesystem::absolute(path);

	// If the model is already loaded, grab the model.
	auto loadedModel = models.find(absolutePath);
	if (loadedModel != models.end())
		return loadedModel->second;

	// Create a new model.
	std::shared_ptr<Model> newModel = std::make_shared<Model>();
	newModel->LoadModel(path);
	models.insert(std::pair<std::filesystem::path, std::shared_ptr<Model>>(absolutePath, newModel));

	return newModel;
}
