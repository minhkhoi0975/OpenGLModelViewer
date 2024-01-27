#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
	Transform(glm::vec3 position = glm::vec3(), glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f))
		: position(position), rotation(rotation), scale(scale) {};

	inline glm::vec3 GetPosition() const { return position; }
	inline void SetPosition(const glm::vec3& newPosition) { position = newPosition; }

	inline glm::quat GetRotation() const { return rotation; }
	inline void SetRotation(const glm::quat& newRotation) { rotation = glm::normalize(newRotation); }

	inline glm::vec3 GetScale() const { return scale; }
	inline void SetScale(const glm::vec3& newScale) { scale = newScale; }

	glm::vec3 GetRightDirection() const;
	glm::vec3 GetUpDirection() const;
	glm::vec3 GetForwardDirection() const;

	glm::mat4 GetTranslationMatrix() const;
	glm::mat4 GetRotationMatrix() const;
	glm::mat4 GetScaleMatrix() const;

	glm::mat4 GetModelMatrix() const;

public:
	glm::vec3 position = glm::vec3();
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};