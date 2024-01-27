#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

out vec3 vertexWorldPosition;
out vec3 vertexWorldNormal;
out vec2 fragmentUV;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);
	vertexWorldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
	vertexWorldNormal = normalMatrix * vertexNormal;
	fragmentUV = vertexUV;
}

