#include "Transform.h"
#include <glm/gtx/quaternion.hpp>

glm::vec3 Transform::GetRightDirection() const
{
	return rotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Transform::GetUpDirection() const
{
	return rotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Transform::GetForwardDirection() const
{
	return rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::mat4 Transform::GetTranslationMatrix() const
{
	return glm::translate(glm::mat4(1.0f), position);
}

glm::mat4 Transform::GetRotationMatrix() const
{
	return glm::toMat4(rotation);
}

glm::mat4 Transform::GetScaleMatrix() const
{
	return glm::scale(glm::mat4(1.0f), scale);
}

glm::mat4 Transform::GetModelMatrix() const
{
	return GetTranslationMatrix() * GetRotationMatrix() * GetScaleMatrix();
}
