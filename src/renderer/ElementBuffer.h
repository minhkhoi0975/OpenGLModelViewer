#pragma once

class ElementBuffer
{
public:
	~ElementBuffer();

	void Init();
	void Shutdown();

	void SetData(const void* data, int size);
	void Use();
	void Unuse();

private:
	unsigned int id = 0;
};