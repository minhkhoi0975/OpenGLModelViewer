#include "Shader.h"
#include "ShaderDataType.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader* Shader::currentShader = nullptr;

Shader::~Shader()
{
	Shutdown();
}

Shader* Shader::GetCurrentShader()
{
	return currentShader;
}

void Shader::Init(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
	// --- Step 1: Open and read the shader files. ---

	std::ifstream vertexShaderFile, fragmentShaderFile;
	std::stringstream vertexShaderSStream, fragmentShaderSStream;

	// Ensure that the input file streams throw exceptions.
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// Try opening and reading the vertex shader source code.
	try
	{
		vertexShaderFile.open(vertexShaderFilePath);
		vertexShaderSStream << vertexShaderFile.rdbuf();
		vertexShaderFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Error: Cannot read the vertex shader source code: " << e.what() << std::endl;
	}

	// Try opening and reading the fragment shader source code.
	try
	{
		fragmentShaderFile.open(fragmentShaderFilePath);
		fragmentShaderSStream << fragmentShaderFile.rdbuf();
		fragmentShaderFile.close();
	}
	catch (std::ifstream::failure e)
	{
		std::cerr << "Error: Cannot read the fragment shader source code: " << e.what() << std::endl;
	}

	// --- Step 2: Create shaders. ---

	unsigned int vertexShader, fragmentShader;
	std::string vertexShaderSourceCode = vertexShaderSStream.str();
	std::string fragmentShaderSourceCode = fragmentShaderSStream.str();
	const char* vertexShaderRawSourceCode = vertexShaderSourceCode.c_str();
	const char* fragmentShaderRawSourceCode = fragmentShaderSourceCode.c_str();
	int success;
	char infoLog[512];

	// Compile the vertex shader.
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderRawSourceCode, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), nullptr, infoLog);
		std::cout << "Cannot compile the vertex shader: " << std::endl << infoLog << std::endl;
	}

	// Compile the fragment shader.
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderRawSourceCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), nullptr, infoLog);
		std::cout << "Cannot compile the fragment shader: " << std::endl << infoLog << std::endl;
	}

	// --- Step 3: Create a shader program. ---

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);
		std::cout << "Cannot link the shaders: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	std::cout << "Compiled shader." << std::endl;
	Use();
	GetAllAttributes();
	GetAllUniforms();
	std::cout << std::endl;
}

void Shader::Shutdown()
{
	Unuse();

	if (id == 0)
		return;

	glDeleteProgram(id);
	id = 0;
}

unsigned int Shader::GetId() const
{
	return id;
}

void Shader::Use()
{
	if (id == 0)
		return;

	glUseProgram(id);
	currentShader = this;
}

void Shader::Unuse() const
{
	if (currentShader != this)
		return;

	glUseProgram(0);
	currentShader = nullptr;
}

void Shader::SetUniformInt(const char* name, const int& value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::SetUniformFloat(const char* name, const float& value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::SetUniformVector3(const char* name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Shader::SetUniformVector4(const char* name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(id, name), 1, &value[0]);
}

void Shader::SetUniformMatrix3(const char* name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformMatrix4(const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::GetAllAttributes()
{
	int attributeCount;
	const GLsizei bufferSize = 16;
	int attributeNameLength;
	char attributeName[bufferSize];
	int attributeSize;
	GLenum attributeType;

	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &attributeCount);

	for (int i = 0; i < attributeCount; ++i)
	{
		glGetActiveAttrib(id, i, bufferSize, &attributeNameLength, &attributeSize, &attributeType, attributeName);

		std::cout << "Attribute name: " << attributeName << ". \tData type: " << GetDataTypeName(attributeType) << "." << std::endl;
	}
}

void Shader::GetAllUniforms()
{
	int uniformCount;
	const GLsizei bufferSize = 16;
	int uniformNameLength;
	char uniformName[bufferSize];
	int uniformSize;
	GLenum uniformType;

	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformCount);

	for (int i = 0; i < uniformCount; ++i)
	{
		glGetActiveUniform(id, i, bufferSize, &uniformNameLength, &uniformSize, &uniformType, uniformName);

		std::cout << "Uniform name: " << uniformName << ". \tData type: " << GetDataTypeName(uniformType) << "." << std::endl;
	}
}
