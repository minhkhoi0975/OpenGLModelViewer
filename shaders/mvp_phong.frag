// References: 
// + https://www.cs.toronto.edu/~jacobson/phong-demo/
// + https://learnopengl.com/Lighting/Basic-Lighting

#version 330 core

struct DirectionalLight 
{
    vec3 directionReverse;
  
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
};

uniform vec3 cameraPosition;

uniform DirectionalLight directionalLight;

uniform float ambientCoefficient = 1.0f;

uniform vec3 diffuseColor = vec3(0.8f, 0.4f, 0.0f);
uniform float diffuseCoefficient = 1.0f;

uniform vec3 specularColor = vec3(1.0f, 1.0f, 1.0f);
uniform float specularCoefficient = 1.0f;
uniform float shininess = 1;

in vec3 vertexWorldPosition;
in vec3 vertexWorldNormal;

out vec4 fragmentColor;

void main()
{
	// Calculate the ambient color.
	fragmentColor = vec4(ambientCoefficient * diffuseColor * directionalLight.ambientColor, 1.0f);

	// Calculate the diffuse color.
	float diffuse = max(dot(vertexWorldNormal, directionalLight.directionReverse), 0.0f);
	fragmentColor += vec4(diffuseCoefficient * diffuse * diffuseColor * directionalLight.diffuseColor, 1.0f);

	// Calculate the specular color.
	vec3 cameraDirectionReverse = normalize(cameraPosition - vertexWorldPosition);
	vec3 lightReflectionDirection = normalize(reflect(-directionalLight.directionReverse, vertexWorldNormal));
	float specular = pow(max(dot(cameraDirectionReverse, lightReflectionDirection), 0.0f), shininess);
	fragmentColor += vec4(specularCoefficient * specular * specularColor * directionalLight.specularColor, 1.0f);
}