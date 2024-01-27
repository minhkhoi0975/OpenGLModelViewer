#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Transform.h"

class Camera
{
public:
	Camera();
	Camera(float fov, float aspectRatio, float nearClipPlane, float farClipPlane);

public:
	inline glm::vec3 GetPosition() { return transform.GetPosition(); }
	inline glm::quat GetRotation() { return transform.GetRotation(); }

	void SetPosition(glm::vec3 newPosition);
	void SetRotation(float pitch, float yaw, float roll, bool anglesInDegrees = true);
	void SetRotation(glm::quat newRotation);

	// Returns the true direction the camera is facing in OpenGL. Don't confuse this with GetForwardDirection().
	glm::vec3 GetFacingDirection() const; 

	// Note: This is the reverse of the camera's facing direction.
	glm::vec3 GetForwardDirection() const;
	glm::vec3 GetRightDirection() const;
	glm::vec3 GetUpDirection() const;

public:
	void SetFov(float fovInDegrees);
	void SetAspectRatio(float aspectRatio);
	void SetNearClipPlane(float nearClipPlane);
	void SetFarClipPlane(float farClipPlane);

public:
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

public:
	Transform transform;

private:
	void RecalculateProjectionMatrix();

private:
	float fov = 45.0f;              // In degrees.
	float aspectRatio;
	float nearClipPlane = 0.1f;
	float farClipPlane = 100.0f;
	glm::mat4 projectionMatrix;
};