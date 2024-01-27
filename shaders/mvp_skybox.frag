#version 330 core

uniform samplerCube cubeMap;

in vec3 textureCoords;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture(cubeMap, textureCoords);
}