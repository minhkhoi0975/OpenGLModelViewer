#version 330 core

layout (location = 0) in vec3 vertexPosition;

out vec3 textureCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	textureCoords = vertexPosition;
	vec4 worldPosition = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0f);
	gl_Position = worldPosition.xyww;
}