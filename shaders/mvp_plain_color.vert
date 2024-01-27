#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 fragmentPosition;
out vec3 normal;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0f);
	fragmentPosition = vec3(modelMatrix * vec4(aPosition, 1.0f));
	normal = normalMatrix * aNormal;
}

