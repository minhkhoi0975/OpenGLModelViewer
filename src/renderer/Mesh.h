#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

#include <vector>
#include <memory>

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<std::shared_ptr<Texture>> textures);
	~Mesh();

	void ClearMesh();

	void Draw(Shader& shader);

public:
	//unsigned int vao, vbo, ebo;
	VertexArray vao;
	VertexBuffer vbo;
	ElementBuffer ebo;

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<std::shared_ptr<Texture>> textures;
};