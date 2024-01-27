#pragma once

class VertexArray
{
public:
	void Init();
	void Shutdown();

	unsigned int GetId();
	void Use();
	void Unuse();

private:
	unsigned int id = 0;
};