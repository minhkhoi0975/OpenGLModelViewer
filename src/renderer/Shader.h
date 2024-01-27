#pragma once
#include <glm/glm.hpp>

class Shader
{
public:
	static Shader* GetCurrentShader();

	~Shader();

	void Init(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	void Shutdown();
	unsigned int GetId() const;

	void Use();
	void Unuse() const;

	void SetUniformInt(const char* name, const int& value);
	void SetUniformFloat(const char* name, const float& value);
	void SetUniformVector3(const char* name, const glm::vec3& value);
	void SetUniformVector4(const char* name, const glm::vec4& value);
	void SetUniformMatrix3(const char* name, const glm::mat3& value);
	void SetUniformMatrix4(const char* name, const glm::mat4& value);

	void GetAllAttributes();
	void GetAllUniforms();

private:
	static Shader* currentShader;

	unsigned int id = -1;
};