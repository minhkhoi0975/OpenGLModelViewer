#pragma once

#include "Mesh.h"
#include "Texture.h"

#include <vector>
#include <cstring>
#include <memory>
#include <filesystem>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:
	void LoadModel(const char* path);
	void ClearModel();

	void Draw(Shader& shader);

	std::filesystem::path GetFilePath() const {return modelFilePath;}

private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::weak_ptr<Texture>> LoadTextures(aiMaterial* material, aiTextureType textureType);
	TextureType GetTextureType(aiTextureType assimpTextureType);

private:
	std::vector<std::shared_ptr<Mesh>> meshes;

	std::filesystem::path modelFilePath;
};