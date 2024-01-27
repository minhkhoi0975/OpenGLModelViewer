#include "Model.h"
#include "Image.h"

#include "../resource_management/ResourceManager.h"

#include <iostream>

void Model::LoadModel(const char* path)
{
	ClearModel();

	// Load model from file.
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals /* | aiProcess_FlipUVs*/);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "Error: Cannot load model from " << path << std::endl;
		return;
	}

	modelFilePath = std::filesystem::absolute(path);

	std::cout 
	<< "Loading model from " << modelFilePath.generic_string() << std::endl
	<< "+ Mesh count: " << scene->mNumMeshes << std::endl
	<< "+ Material count: " << scene->mNumMaterials << std::endl
	<< "+ Texture count: " << scene->mNumTextures << std::endl
	<< std::endl;

	ProcessNode(scene->mRootNode, scene);
}

void Model::ClearModel()
{
	meshes.clear();
}

void Model::Draw(Shader& shader)
{
	for (unsigned int i = 0, ni = meshes.size(); i < ni; ++i)
		meshes[i]->Draw(shader);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// Process the meshes of the node.
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}

	// Process the meshes of the child node.
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
		ProcessNode(node->mChildren[i], scene);
}

std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<std::shared_ptr<Texture>> textures;

	// Process the vertices.
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->HasTextureCoords(0))
		{
			vertex.textureCoordinates.x = mesh->mTextureCoords[0][i].x;
			vertex.textureCoordinates.y = mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	// Process the indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; ++j)
			indicies.push_back(face.mIndices[j]);
	}

	// Process the textures.
	aiMaterial* material = nullptr;
	if (mesh->mMaterialIndex > 0)
		material = scene->mMaterials[mesh->mMaterialIndex];
	// In case the mesh has no assigned material, try assigning the first material of the scene to the mesh.
	else if (scene->mNumMaterials > 0)
		material = scene->mMaterials[0];

	if (material)
	{
		// Add diffuse maps.
		std::vector<std::weak_ptr<Texture>> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// Add specular maps.
		std::vector<std::weak_ptr<Texture>> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		// Add normal maps.
		std::vector< std::weak_ptr<Texture>> normalMaps = LoadTextures(material, aiTextureType_NORMALS);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return std::make_shared<Mesh>(vertices, indicies, textures);
}

std::vector<std::weak_ptr<Texture>> Model::LoadTextures(aiMaterial* material, aiTextureType textureType)
{
	std::vector<std::weak_ptr<Texture>> textures;

	for (unsigned int i = 0; i < material->GetTextureCount(textureType); ++i)
	{
		aiString assimpTextureFilePath;
		material->GetTexture(textureType, i, &assimpTextureFilePath);

		// Only keep the file name of the texture instead of using the whole path.
		std::filesystem::path originalTextureFilePath = assimpTextureFilePath.C_Str();

		std::string textureFilePath = modelFilePath.parent_path().generic_string() + "/" + originalTextureFilePath.filename().generic_string();

		/*
		// Don't load the texture if it has already been loaded.
		bool isTextureAlreadyLoaded = false;
		for (unsigned int j = 0; j < loadedTextures.size(); ++j)
		{
			if (loadedTextures[j]->GetFilePath().compare(textureFilePath) == 0)
			{
				textures.push_back(loadedTextures[j]);
				isTextureAlreadyLoaded = true;
				break;
			}
		}

		if (isTextureAlreadyLoaded)
			continue;

		std::cout << "Texture not loaded yet: " << textureFilePath << std::endl;

		// Texture needs to be loaded from file.
		Image image(textureFilePath.c_str(), true);

		std::shared_ptr<Texture> texture = std::make_shared<Texture>();
		texture->Init();
		texture->LoadImage(&image, GetTextureType(textureType));
		textures.push_back(texture);
		loadedTextures.push_back(texture);
		*/

		std::weak_ptr<Texture> texture = ResourceManager::GetInstance()->GetTexture(textureFilePath.c_str(), GetTextureType(textureType));
		textures.push_back(texture);
	}

	return textures;
}

TextureType Model::GetTextureType(aiTextureType assimpTextureType)
{
	switch (assimpTextureType)
	{
	case aiTextureType_DIFFUSE:
		return TextureType::Diffuse;
	case aiTextureType_SPECULAR:
		return TextureType::Specular;
	case aiTextureType_NORMALS:
		return TextureType::Normal;
	}

	return TextureType::Unknown;
}
