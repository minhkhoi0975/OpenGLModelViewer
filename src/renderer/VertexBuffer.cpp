#include "VertexBuffer.h"

VertexBuffer::~VertexBuffer()
{
	Shutdown();
}

void VertexBuffer::Init()
{
	if (id != 0)
		return;

	glGenBuffers(1, &id);
}

void VertexBuffer::Shutdown()
{
	if (id == 0)
		return;

	glDeleteBuffers(1, &id);
	id = 0;
}

void VertexBuffer::SetData(const void* data, int size, int usage)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexBuffer::Use()
{
	if (id == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::Unuse()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}