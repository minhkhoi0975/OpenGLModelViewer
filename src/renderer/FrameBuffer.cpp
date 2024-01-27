#include "FrameBuffer.h"
#include <glad/glad.h>

FrameBuffer::~FrameBuffer()
{
	Shutdown();
}

void FrameBuffer::Init()
{
	glGenFramebuffers(1, &id);
}

void FrameBuffer::Shutdown()
{
	glDeleteFramebuffers(1, &id);
}

void FrameBuffer::Use()
{
	if (id == 0)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBuffer::Unuse()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
