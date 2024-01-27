#include "VertexArray.h"
#include <glad/glad.h>

void VertexArray::Init()
{
	glGenVertexArrays(1, &id);
}

void VertexArray::Shutdown()
{
	glDeleteVertexArrays(1, &id);
}

unsigned int VertexArray::GetId()
{
	return id;
}

void VertexArray::Use()
{
	if (id == 0)
		return;

	glBindVertexArray(id);
}

void VertexArray::Unuse()
{
	glBindVertexArray(0);
}