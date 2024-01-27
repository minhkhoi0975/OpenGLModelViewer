#include "ElementBuffer.h"
#include <glad/glad.h>

ElementBuffer::~ElementBuffer()
{
	Shutdown();
}

void ElementBuffer::Init()
{
	if (id != 0)
		return;

	glGenBuffers(1, &id);
}

void ElementBuffer::Shutdown()
{
	if (id == 0)
		return;

	glDeleteBuffers(1, &id);
	id = 0;
}

void ElementBuffer::SetData(const void* data, int size)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void ElementBuffer::Use()
{
	if (id != 0)
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ElementBuffer::Unuse()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
