#version 330 core

uniform vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

in vec3 fragmentPosition;
in vec3 normal;

out vec4 fragmentColor;

void main()
{
	fragmentColor = color;
}