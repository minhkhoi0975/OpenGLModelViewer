#pragma once

#include <list>
#include <vector>

#include <glm/glm.hpp>

#include "RendererSettings.h"

#include "../core/Window.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

#include "Camera.h"
#include "Transform.h"

#include "Shader.h"
#include "DirectionalLight.h"
#include "Model.h"
#include "ModelInstance.h"
#include "CubeMap.h"

class Renderer
{
public:
	void Init(Window* window);
	void Update(double deltaTime);
	void Shutdown();

	Window* GetWindow();

	void ApplySettings(RendererSettings settings);
	void SetWireframeModeEnabled(bool isEnabled);
	void SetDepthTestingEnabled(bool isEnabled);
	void SetFaceCullingEnabled(bool isEnabled);
	void SetFaceToCull(bool backFace = true);
	void SetFrontFaceOrder(bool counterClockwise = true);

	void SetModel(const char* path);

	void LoadModel(const char* path);
	void RemoveModelAt(int index);
	std::vector<std::shared_ptr<Model>> GetLoadedModels() const {return loadedModels;}

	void CreateModelInstance(int modelIndex);
	void RemoveModelInstance(int modelInstanceIndex);
	std::vector<std::shared_ptr<ModelInstance>> GetModelInstances() const {return modelInstances;}

public:
	float cameraMoveSpeed = 5.0f;
	float cameraRotateSpeed = 10.0f;

	glm::vec4 backgroundColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

	DirectionalLight directionalLight;

	bool shouldDrawSkybox = true;
	bool shouldDrawGrid = true;

	bool shouldDrawCube = true;
	glm::vec3 cubePosition = glm::vec3(-3.0f, 0.0f, 0.0f);
	glm::vec3 cubeRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cubeScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 cubeColor = glm::vec3(0.8f, 0.0f, 0.0f);

	bool shouldDrawModel = true;
	glm::vec3 modelPosition = glm::vec3(3.0f, 0.0f, 0.0f);
	glm::vec3 modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 modelScale = glm::vec3(1.0f, 1.0f, 1.0f);
	float modelAmbientCoefficient = 1.0f;
	glm::vec3 modelDiffuseColor = glm::vec3(0.8f, 0.4f, 0.0f);
	float modelDiffuseCoefficient = 1.0f;
	glm::vec3 modelSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float modelSpecularCoefficient = 1.0f;
	float modelShiniess = 32.0f;

	bool shouldDrawModel2 = true;
	glm::vec3 model2Position = glm::vec3(3.0f, 0.0f, 6.0f);
	glm::vec3 model2Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 model2Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float model2AmbientCoefficient = 1.0f;
	float model2DiffuseCoefficient = 1.0f;
	float model2SpecularCoefficient = 1.0f;
	float model2Shiniess = 32.0f;

private:
	Camera camera;
	bool shouldRotateCamera = false;
	float cameraYaw = 0.0f, cameraPitch = 0.0f;
	glm::vec2 cameraMoveInput = glm::vec2();

private:
	VertexArray vertexArray;

private:
	VertexBuffer gridVertexBuffer;
	ElementBuffer gridElementBuffer;
	std::weak_ptr<Shader> gridShader;

	// Define the vertices of the grid plane.
	float gridVertices[12] =
	{
		-10.0f, 0.0f, -10.0f,
		 10.0f, 0.0f,  10.0f,
		-10.0f, 0.0f,  10.0f,
		 10.0f, 0.0f, -10.0f
	};

	int gridIndices[6] = 
	{
		2, 1, 0,
		1, 3, 0
	};

private:
	VertexBuffer skyboxVertexBuffer;
	CubeMap skyboxCubeMap;
	std::weak_ptr<Shader> skyboxShader;

	const char* cubeMapImages[6] =
	{
		"models/skybox_right.jpg",
		"models/skybox_left.jpg",
		"models/skybox_top.jpg",
		"models/skybox_bottom.jpg",
		"models/skybox_front.jpg",
		"models/skybox_back.jpg"
	};

	float skyboxTextureCoordinates[108] =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

private:
	VertexBuffer cubeVertexBuffer;
	std::weak_ptr<Shader> cubeShader;

	// Define the vertices of a cube.
	float cubeVertices[216] =
	{
		// Positions         // Normals

		// Back face
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   // Front face
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		// Left face
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	    // Right face
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		// Bottom face
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   // Top face
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	};
	Transform cubeModelMatrix;
	glm::mat3 cubeNormalMatrix;

private:
	std::weak_ptr<Model> model;
	std::weak_ptr<Shader> modelShader;
	Transform modelModelMatrix;
	glm::mat3 modelNormalMatrix;

	std::weak_ptr<Shader> modelShader2;
	Transform model2ModelMatrix;
	glm::mat3 model2NormalMatrix;

	std::vector<std::shared_ptr<Model>> loadedModels;
	std::vector<std::shared_ptr<ModelInstance>> modelInstances;

private:
	void UpdateMatrices();
	void MoveCamera(double deltaTime);
	void Draw();

private:
	void UpdateCursorData();
	void OnWindowResized(int newWidth, int newHeight);
	void OnKeyPressed(int key);
	void OnKeyReleased(int key);
	void OnMouseButtonPressed(int button);
	void OnMouseButtonReleased(int button);

	void BindFunctionsToWindow();
	void UnbindFunctionsFromWindow();

	double cursorPositionX = 0.0f, cursorPositionY = 0.0f;
	double cursorDeltaX = 0.0f, cursorDeltaY = 0.0f;

	Window* window;
	std::list<std::function<void(int, int)>>::iterator onWindowResizedIterator;
	std::list<std::function<void(int)>>::iterator onKeyPressedIterator;
	std::list<std::function<void(int)>>::iterator onKeyReleasedIterator;
	std::list<std::function<void(int)>>::iterator onMouseButtonPressedIterator;
	std::list<std::function<void(int)>>::iterator onMouseButtonReleasedIterator;
};