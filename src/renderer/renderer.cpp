#include "renderer.h"
#include "../resource_management/ResourceManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void Renderer::Init(Window* window)
{
	// Initialize GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Error: Cannot initialize GLAD. " << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	// Ensure that the renderer has target window.
	if (window == nullptr)
	{
		std::cerr << "Error: Renderer has no target window." << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	
	this->window = window;
	BindFunctionsToWindow();

	// Initialize the directional light.
	{
		directionalLight.direction = glm::vec3(1.0f, -1.0f, 1.0f);
		directionalLight.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
		directionalLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		directionalLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	// Initialize the default vertex array object.
	vertexArray.Init();
	vertexArray.Use();

	{
		// Initialize the skybox's vertex buffer.
		skyboxVertexBuffer.Init();
		skyboxVertexBuffer.Use();
		skyboxVertexBuffer.SetData(&skyboxTextureCoordinates, sizeof(skyboxTextureCoordinates));

		// Initialize the skybox's cubemap.
		skyboxCubeMap.Init();
		skyboxCubeMap.LoadImages(cubeMapImages);

		// Initialize the skybox's shader.
		skyboxShader = ResourceManager::GetInstance()->LoadShader("skybox", "shaders/mvp_skybox.vert", "shaders/mvp_skybox.frag");
	}

	{
		// Initialize the grid's array buffer.
		gridVertexBuffer.Init();
		gridVertexBuffer.Use();
		gridVertexBuffer.SetData(&gridVertices, sizeof(gridVertices));

		// Initialize the grid's element buffer.
		gridElementBuffer.Init();
		gridElementBuffer.Use();
		gridElementBuffer.SetData(&gridIndices, sizeof(gridIndices));

		// Initialize the grid's shader.
		gridShader = ResourceManager::GetInstance()->LoadShader("grid", "shaders/mvp_grid.vert", "shaders/mvp_grid.frag");
	}

	{
		// Initialize the cube's array buffer.
		cubeVertexBuffer.Init();
		cubeVertexBuffer.Use();
		cubeVertexBuffer.SetData(&cubeVertices, sizeof(cubeVertices));

		// Initialize the cube's shader.
		cubeShader = ResourceManager::GetInstance()->LoadShader("diffuse_only", "shaders/mvp_diffuse.vert", "shaders/mvp_diffuse.frag");
	}

	{
		// Initialize the model.
		//model.LoadModel("models/backpack.obj");
		model = ResourceManager::GetInstance()->GetModel("models/backpack.obj");

		// Initialize the model's shader.
		modelShader = ResourceManager::GetInstance()->LoadShader("phong", "shaders/mvp_diffuse.vert", "shaders/mvp_phong.frag");

		// Initialize the Phong shader that uses textures.
		modelShader2 = ResourceManager::GetInstance()->LoadShader("phong_with_texture", "shaders/mvp_diffuse.vert", "shaders/mvp_phong_texture.frag");
	}

	// Initialize the camera.
	int windowWidth, windowHeight;
	window->GetWindowSize(&windowWidth, &windowHeight);
	camera = Camera(45.0f, (float)windowWidth / windowHeight, 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
}

void Renderer::Update(double deltaTime)
{
	UpdateCursorData();

	UpdateMatrices();

	MoveCamera(deltaTime);

	Draw();
}

void Renderer::Shutdown()
{
	if (auto modelShader2Shared = modelShader2.lock())
		modelShader2Shared->Shutdown();

	if (auto modelShaderShared = modelShader.lock())
		modelShaderShared->Shutdown();

	if (auto modelShared = model.lock())
		modelShared->ClearModel();

	if (auto cubeShaderShared = cubeShader.lock())
		cubeShaderShared->Shutdown();
	cubeVertexBuffer.Shutdown();

	if (auto gridShaderShared = gridShader.lock())
		gridShaderShared->Shutdown();
	gridElementBuffer.Shutdown();
	gridVertexBuffer.Shutdown();

	if (auto skyboxShaderShared = skyboxShader.lock())
		skyboxShaderShared->Shutdown();
	skyboxCubeMap.Shutdown();
	skyboxVertexBuffer.Shutdown();

	vertexArray.Shutdown();

	UnbindFunctionsFromWindow();
}

Window* Renderer::GetWindow()
{
	return window;
}

// ------------------
// Rendering Settings
// ------------------

void Renderer::ApplySettings(RendererSettings settings)
{
	SetWireframeModeEnabled(settings.enableWireframeMode);
	SetDepthTestingEnabled(settings.enableDepthTesting);
	SetFaceCullingEnabled(settings.enableFaceCulling);
	SetFaceToCull(settings.isCulledFaceBack);
	SetFrontFaceOrder(settings.isFrontFaceOrderCCW);
}

void Renderer::SetWireframeModeEnabled(bool isEnabled)
{
	if (isEnabled)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::SetDepthTestingEnabled(bool isEnabled)
{
	if (isEnabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void Renderer::SetFaceCullingEnabled(bool isEnabled)
{
	if (isEnabled)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Renderer::SetFaceToCull(bool backFace)
{
	if (backFace)
		glCullFace(GL_BACK);
	else
		glCullFace(GL_FRONT);
}

void Renderer::SetFrontFaceOrder(bool counterClockwise)
{
	if (counterClockwise)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
}

void Renderer::SetModel(const char* path)
{
	model = ResourceManager::GetInstance()->GetModel(path);
}

void Renderer::LoadModel(const char* path)
{
	std::shared_ptr<Model> model = std::make_shared<Model>();
	model->LoadModel(path);
	loadedModels.push_back(model);
}

void Renderer::RemoveModelAt(int index)
{
	if (index < 0 || index > loadedModels.size())
		return;

	std::shared_ptr<Model> modelToRemove = loadedModels[index];

	loadedModels.erase(loadedModels.begin() + index);

	for (int i = 0; i < modelInstances.size(); ++i)
	{
		if (auto modelSharedPointer = modelInstances[i]->GetModel().lock())
		{ 
			if (modelSharedPointer == modelToRemove)
			{
				modelInstances.erase(modelInstances.begin() + i);
				--i;
			}
		}
		else
		{
			modelInstances.erase(modelInstances.begin() + i);
			--i;
		}
	}
}

void Renderer::CreateModelInstance(int modelIndex)
{
	if (modelIndex < 0 || modelIndex >= loadedModels.size() || loadedModels[modelIndex] == nullptr)
		return;

	std::shared_ptr<ModelInstance> modelInstance = std::make_shared<ModelInstance>();
	modelInstance->SetModel(loadedModels[modelIndex]);

	modelInstances.push_back(modelInstance);
}

void Renderer::RemoveModelInstance(int modelInstanceIndex)
{
	if (modelInstanceIndex < 0 || modelInstanceIndex >= modelInstances.size())
		return;

	modelInstances.erase(modelInstances.begin() + modelInstanceIndex);
}

// -------------
// Cursor
// -------------

void Renderer::UpdateCursorData()
{
	// Update cursor's position and delta.
	double newCursorPositionX, newCursorPositionY;
	window->GetCursorPosition(&newCursorPositionX, &newCursorPositionY);

	cursorDeltaX = newCursorPositionX - cursorPositionX;
	cursorDeltaY = newCursorPositionY - cursorPositionY;

	cursorPositionX = newCursorPositionX;
	cursorPositionY = newCursorPositionY;
}

// -------------
// Camera
// -------------

void Renderer::MoveCamera(double deltaTime)
{
	// Update position.
	camera.SetPosition(camera.GetPosition() + cameraMoveInput.y * camera.GetFacingDirection() * cameraMoveSpeed * (float)deltaTime);
	camera.SetPosition(camera.GetPosition() + cameraMoveInput.x * camera.GetRightDirection() * cameraMoveSpeed * (float)deltaTime);

	// Update rotation.
	if (shouldRotateCamera)
	{
		// Update the yaw.
		cameraYaw += -cursorDeltaX * cameraRotateSpeed * deltaTime;

		// Update the pitch.
		cameraPitch += -cursorDeltaY * cameraRotateSpeed * deltaTime;

		camera.SetRotation(cameraPitch, cameraYaw, 0.0f);
	}
}

// ----------
// Drawing
// ----------

void Renderer::UpdateMatrices()
{
	// Update the cube's model matrix.
	cubeModelMatrix.SetPosition(cubePosition);
	cubeModelMatrix.SetRotation(glm::quat(glm::vec3(glm::radians(cubeRotation.x), glm::radians(cubeRotation.y), glm::radians(cubeRotation.z))));
	cubeModelMatrix.SetScale(cubeScale);

	// Update the cube's normal matrix.
	cubeNormalMatrix = glm::mat3(transpose(glm::inverse(camera.GetViewMatrix() * cubeModelMatrix.GetModelMatrix())));

	// Update the model's model matrix.
	modelModelMatrix.SetPosition(modelPosition);
	modelModelMatrix.SetRotation(glm::quat(glm::vec3(glm::radians(modelRotation.x), glm::radians(modelRotation.y), glm::radians(modelRotation.z))));
	modelModelMatrix.SetScale(modelScale);

	// Update the model's normal matrix.
	modelNormalMatrix = glm::mat3(transpose(glm::inverse(modelModelMatrix.GetModelMatrix())));

	// Update the 2nd model's model matrix.
	model2ModelMatrix.SetPosition(model2Position);
	model2ModelMatrix.SetRotation(glm::quat(glm::vec3(glm::radians(model2Rotation.x), glm::radians(model2Rotation.y), glm::radians(model2Rotation.z))));
	model2ModelMatrix.SetScale(model2Scale);

	// Update the model's normal matrix.
	model2NormalMatrix = glm::mat3(transpose(glm::inverse(model2ModelMatrix.GetModelMatrix())));
}

void Renderer::Draw()
{
	// Clear the scene.
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw the grid.
	if (shouldDrawGrid)
	{ 
		vertexArray.Use();
		gridVertexBuffer.Use();
		gridElementBuffer.Use();

		if (auto gridShaderShared = gridShader.lock())
		{ 
			gridShaderShared->Use();

			gridShaderShared->SetUniformMatrix4("modelMatrix", glm::mat4(1.0f));
			gridShaderShared->SetUniformMatrix4("viewMatrix", camera.GetViewMatrix());
			gridShaderShared->SetUniformMatrix4("projectionMatrix", camera.GetProjectionMatrix());
			gridShaderShared->SetUniformMatrix3("normalMatrix", cubeNormalMatrix);

			gridShaderShared->SetUniformVector3("color", glm::vec3(0.0f, 0.0f, 0.8f));
		}

		// Set the positions of the grid's vertices.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// Draw the cube.
	if (shouldDrawCube)
	{
		vertexArray.Use();
		cubeVertexBuffer.Use();

		if (auto cubeShaderShared = cubeShader.lock())
		{
			cubeShaderShared->Use();

			cubeShaderShared->SetUniformMatrix4("modelMatrix", cubeModelMatrix.GetModelMatrix());
			cubeShaderShared->SetUniformMatrix4("viewMatrix", camera.GetViewMatrix());
			cubeShaderShared->SetUniformMatrix4("projectionMatrix", camera.GetProjectionMatrix());
			cubeShaderShared->SetUniformMatrix3("normalMatrix", cubeNormalMatrix);

			cubeShaderShared->SetUniformVector3("lightDirectionReverse", glm::normalize(-directionalLight.direction));
			cubeShaderShared->SetUniformVector3("color", cubeColor);
		}

		// Set the positions of the vertices.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Set the normals of the vertices.
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Draw the cube.
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Draw the model.
	if (shouldDrawModel)
	{
		if (auto modelShaderShared = modelShader.lock())
		{ 
			modelShaderShared->Use();

			modelShaderShared->SetUniformMatrix4("modelMatrix", modelModelMatrix.GetModelMatrix());
			modelShaderShared->SetUniformMatrix4("viewMatrix", camera.GetViewMatrix());
			modelShaderShared->SetUniformMatrix4("projectionMatrix", camera.GetProjectionMatrix());
			modelShaderShared->SetUniformMatrix3("normalMatrix", modelNormalMatrix);

			modelShaderShared->SetUniformVector3("cameraPosition", camera.GetPosition());

			modelShaderShared->SetUniformVector3("directionalLight.directionReverse", glm::normalize(-directionalLight.direction));
			modelShaderShared->SetUniformVector3("directionalLight.ambientColor", directionalLight.ambient);
			modelShaderShared->SetUniformVector3("directionalLight.diffuseColor", directionalLight.diffuse);
			modelShaderShared->SetUniformVector3("directionalLight.specularColor", directionalLight.specular);

			modelShaderShared->SetUniformFloat("ambientCoefficient", modelAmbientCoefficient);
			modelShaderShared->SetUniformVector3("diffuseColor", modelDiffuseColor);
			modelShaderShared->SetUniformFloat("diffuseCoefficient", modelDiffuseCoefficient);
			modelShaderShared->SetUniformVector3("specularColor", modelSpecularColor);
			modelShaderShared->SetUniformFloat("specularCoefficient", modelSpecularCoefficient);
			modelShaderShared->SetUniformFloat("shininess", modelShiniess);

			if (auto modelShared = model.lock())
				modelShared->Draw(*modelShaderShared);
		}
	}

	// Draw the 2nd model.
	if (shouldDrawModel2)
	{
		if (auto modelShader2Shared = modelShader2.lock())
		{ 
			modelShader2Shared->Use();

			modelShader2Shared->SetUniformMatrix4("modelMatrix", model2ModelMatrix.GetModelMatrix());
			modelShader2Shared->SetUniformMatrix4("viewMatrix", camera.GetViewMatrix());
			modelShader2Shared->SetUniformMatrix4("projectionMatrix", camera.GetProjectionMatrix());
			modelShader2Shared->SetUniformMatrix3("normalMatrix", model2NormalMatrix);

			modelShader2Shared->SetUniformVector3("cameraPosition", camera.GetPosition());

			modelShader2Shared->SetUniformVector3("directionalLight.directionReverse", glm::normalize(-directionalLight.direction));
			modelShader2Shared->SetUniformVector3("directionalLight.ambientColor", directionalLight.ambient);
			modelShader2Shared->SetUniformVector3("directionalLight.diffuseColor", directionalLight.diffuse);
			modelShader2Shared->SetUniformVector3("directionalLight.specularColor", directionalLight.specular);

			modelShader2Shared->SetUniformFloat("ambientCoefficient", model2AmbientCoefficient);
			modelShader2Shared->SetUniformFloat("diffuseCoefficient", model2DiffuseCoefficient);
			modelShader2Shared->SetUniformFloat("specularCoefficient", model2SpecularCoefficient);
			modelShader2Shared->SetUniformFloat("shininess", model2Shiniess);

			if (auto modelShared = model.lock())
				modelShared->Draw(*modelShader2Shared);
		}
	}

	if (auto modelShader2Shared = modelShader2.lock())
		for (int i = 0; i < modelInstances.size(); ++i)
			modelInstances[i]->Draw(camera, directionalLight, *modelShader2Shared);

	// Draw the skybox.
	if (shouldDrawSkybox)
	{
		glDepthFunc(GL_LEQUAL);

		vertexArray.Use();
		skyboxVertexBuffer.Use();
		skyboxCubeMap.Use();

		if (auto skyboxShaderShared = skyboxShader.lock())
		{ 
			skyboxShaderShared->Use();
			skyboxShaderShared->SetUniformMatrix4("viewMatrix", glm::mat4(glm::mat3(camera.GetViewMatrix()))); // Remove translation from the view matrix.
			skyboxShaderShared->SetUniformMatrix4("projectionMatrix", camera.GetProjectionMatrix());
		}

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDepthMask(GL_TRUE);

		glDepthFunc(GL_LESS);
	}
}

// -------------
// Events
// -------------

void Renderer::OnWindowResized(int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);

	if (newHeight < 1)
		newHeight = 1;
	camera.SetAspectRatio((float)newWidth/newHeight);
}

void Renderer::OnKeyPressed(int key)
{
	switch (key)
	{
	case GLFW_KEY_W:
		cameraMoveInput.y += 1.0f;
		break;
	case GLFW_KEY_S:
		cameraMoveInput.y -= 1.0f;
		break;
	case GLFW_KEY_D:
		cameraMoveInput.x += 1.0f;
		break;
	case GLFW_KEY_A:
		cameraMoveInput.x -= 1.0f;
		break;

	case GLFW_KEY_ESCAPE:
		window->CloseWindow();
		break;
	}
}

void Renderer::OnKeyReleased(int key)
{
	switch (key)
	{
	case GLFW_KEY_W:
		cameraMoveInput.y -= 1.0f;
		break;
	case GLFW_KEY_S:
		cameraMoveInput.y += 1.0f;
		break;
	case GLFW_KEY_D:
		cameraMoveInput.x -= 1.0f;
		break;
	case GLFW_KEY_A:
		cameraMoveInput.x += 1.0f;
		break;
	}
}

void Renderer::OnMouseButtonPressed(int button)
{
	switch (button)
	{
		case 1:
			shouldRotateCamera = true;
			break;
	}
}

void Renderer::OnMouseButtonReleased(int button)
{
	switch (button)
	{
	case 1:
		shouldRotateCamera = false;
		break;
	}
}

void Renderer::BindFunctionsToWindow()
{
	if (!window)
		return;

	onWindowResizedIterator = window->onWindowResized.Bind(std::bind(&Renderer::OnWindowResized, this, std::placeholders::_1, std::placeholders::_2));
	onKeyPressedIterator = window->onKeyPressed.Bind(std::bind(&Renderer::OnKeyPressed, this, std::placeholders::_1));
	onKeyReleasedIterator = window->onKeyReleased.Bind(std::bind(&Renderer::OnKeyReleased, this, std::placeholders::_1));
	onMouseButtonPressedIterator = window->onMousePressed.Bind(std::bind(&Renderer::OnMouseButtonPressed, this, std::placeholders::_1));
	onMouseButtonReleasedIterator = window->onMouseReleased.Bind(std::bind(&Renderer::OnMouseButtonReleased, this, std::placeholders::_1));
}

void Renderer::UnbindFunctionsFromWindow()
{
	if (!window)
		return;

	window->onMouseReleased.Unbind(onMouseButtonReleasedIterator);
	window->onMousePressed.Unbind(onMouseButtonPressedIterator);
	window->onKeyReleased.Unbind(onKeyReleasedIterator);
	window->onKeyPressed.Unbind(onKeyPressedIterator);
	window->onWindowResized.Unbind(onWindowResizedIterator);
}