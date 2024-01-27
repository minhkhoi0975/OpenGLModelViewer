#include "Mesh.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<std::shared_ptr<Texture>> textures)
	: vertices(vertices), indicies(indicies), textures(textures)
{
	vao.Init();
	vao.Use();

	vbo.Init();
	vbo.Use();
	vbo.SetData(&vertices[0], vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);

	ebo.Init();
	ebo.Use();
	ebo.SetData(&indicies[0], indicies.size() * sizeof(unsigned int));

	// Set the positions.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	// Set the normals.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	// Set the texture coordinates.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}

Mesh::~Mesh()
{
	ClearMesh();
}

void Mesh::ClearMesh()
{
	ebo.Shutdown();
	vbo.Shutdown();
	vao.Shutdown();

	vertices.clear();
	indicies.clear();
	textures.clear();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseIndex = 0;
	unsigned int specularIndex = 0;

	for (int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string variableName = "";
		TextureType textureType = textures[i]->GetTextureType();

		if (textureType == TextureType::Diffuse)
			variableName = "diffuseMap";
		else if (textureType == TextureType::Specular)
			variableName = "specularMap";
		else // Skip unknown textures.
			continue;

		shader.SetUniformInt(variableName.c_str(), i);

		textures[i]->Use();
	}

	glActiveTexture(GL_TEXTURE0);

	vao.Use();
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}