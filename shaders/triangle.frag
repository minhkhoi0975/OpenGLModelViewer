#version 330 core

uniform vec3 color = vec3(1.0f, 0.5f, 0.2f);
							
out vec4 fragColor;							
											
void main()									
{											
	fragColor = vec4(color, 1.0f);
}