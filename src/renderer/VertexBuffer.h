#pragma once
#include <glad/glad.h>

class VertexBuffer
{
public:
	~VertexBuffer();

	void Init();
	void Shutdown();

	void SetData(const void* data, int size, int usage = GL_STATIC_DRAW);
	void Use();
	void Unuse();

private:
	unsigned int id = 0;
};