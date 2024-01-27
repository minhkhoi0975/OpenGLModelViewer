#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

Camera::Camera() {}

Camera::Camera(float fovInDegrees, float aspectRatio, float nearClipPlane, float farClipPlane)
	: fov(fovInDegrees), aspectRatio(aspectRatio), nearClipPlane(nearClipPlane), farClipPlane(farClipPlane)
{
	RecalculateProjectionMatrix();
}

glm::vec3 Camera::GetFacingDirection() const
{
	// Note: In OpenGL, cameras face backwards in their coordinate system.
	return -transform.GetForwardDirection();
}

glm::vec3 Camera::GetForwardDirection() const
{
	return transform.GetForwardDirection();
}

glm::vec3 Camera::GetRightDirection() const
{
	return transform.GetRightDirection();
}

glm::vec3 Camera::GetUpDirection() const
{
	return transform.GetUpDirection();
}

void Camera::SetPosition(glm::vec3 newPosition)
{
	transform.SetPosition(newPosition);
}

void Camera::SetRotation(float pitch, float yaw, float roll, bool anglesInDegrees)
{
	if (anglesInDegrees)
	{
		pitch = glm::radians(pitch);
		yaw = glm::radians(yaw);
		roll = glm::radians(roll);
	}

	transform.SetRotation(glm::quat(glm::vec3(pitch, yaw, roll)));
}

void Camera::SetRotation(glm::quat newRotation)
{
	newRotation = glm::normalize(newRotation);
	transform.SetRotation(newRotation);
}

void Camera::SetFov(float fovInDegrees)
{
	this->fov = fovInDegrees;
	RecalculateProjectionMatrix();
}

void Camera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
	RecalculateProjectionMatrix();
}

void Camera::SetNearClipPlane(float nearClipPlane)
{
	this->nearClipPlane = nearClipPlane;
	RecalculateProjectionMatrix();
}

void Camera::SetFarClipPlane(float farClipPlane)
{
	this->farClipPlane = farClipPlane;
	RecalculateProjectionMatrix();
}

glm::mat4 Camera::GetViewMatrix() const
{
	// Calculate the camera's model matrix.
	glm::mat4 cameraModelMatrix = transform.GetTranslationMatrix() * transform.GetRotationMatrix();

	// Inverse the matrix to get the view matrix.
	return glm::inverse(cameraModelMatrix);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void Camera::RecalculateProjectionMatrix()
{
	projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClipPlane, farClipPlane);
}