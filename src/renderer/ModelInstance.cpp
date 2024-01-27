#include "ModelInstance.h"

void ModelInstance::SetModel(std::weak_ptr<Model> model)
{
	this->model = model;
}

void ModelInstance::Draw(Camera camera, DirectionalLight directionalLight, Shader& shader)
{
	std::shared_ptr<Model> modelShared = model.lock();
	if (!modelShared)
		return;

	shader.Use();

	// Set the matrices.
	shader.SetUniformMatrix4("modelMatrix", transform.GetModelMatrix());
	shader.SetUniformMatrix4("viewMatrix", camera.GetViewMatrix());
	shader.SetUniformMatrix4("projectionMatrix", camera.GetProjectionMatrix());
	glm::mat3 normalMatrix = glm::mat3(transpose(glm::inverse(transform.GetModelMatrix())));
	shader.SetUniformMatrix3("normalMatrix", normalMatrix);

	// Set camera property.
	shader.SetUniformVector3("cameraPosition", camera.GetPosition());

	// Set light properties.
	shader.SetUniformVector3("directionalLight.directionReverse", glm::normalize(-directionalLight.direction));
	shader.SetUniformVector3("directionalLight.ambientColor", directionalLight.ambient);
	shader.SetUniformVector3("directionalLight.diffuseColor", directionalLight.diffuse);
	shader.SetUniformVector3("directionalLight.specularColor", directionalLight.specular);

	// Set Phong properties.
	shader.SetUniformFloat("ambientCoefficient", ambientCoefficient);
	shader.SetUniformFloat("diffuseCoefficient", diffuseCoefficient);
	shader.SetUniformVector3("diffuseColor", diffuseColor);
	shader.SetUniformFloat("specularCoefficient", specularCoefficient);
	shader.SetUniformVector3("specularColor", specularColor);
	shader.SetUniformFloat("shininess", shininess);

	modelShared->Draw(shader);
}
