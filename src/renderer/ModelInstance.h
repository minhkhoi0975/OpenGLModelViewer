#pragma once

#include "Model.h"
#include "Transform.h"
#include "Shader.h"
#include "Camera.h"
#include "DirectionalLight.h"

#include <memory>

class ModelInstance
{
public:
	Transform transform;

	float ambientCoefficient = 1.0f;

	float diffuseCoefficient = 1.0f;
	glm::vec3 diffuseColor = glm::vec3(0.8f, 0.4f, 0.0f);

	float specularCoefficient = 1.0f;
	glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);

	float shininess = 32.0f;

	std::weak_ptr<Model> GetModel() const {return model;}
	void SetModel(std::weak_ptr<Model> model);

	void Draw(Camera camera, DirectionalLight directionalLight, Shader& shader);

private:
	std::weak_ptr<Model> model;
};