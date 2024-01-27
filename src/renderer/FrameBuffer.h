#pragma once

class FrameBuffer
{
public:
	~FrameBuffer();

	void Init();
	void Shutdown();

	void Use();
	void Unuse();

private:
	unsigned int id = 0;
};