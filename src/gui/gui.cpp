#include "gui.h"
#include "filedialog.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>

void Gui::Init(Window* window)
{
	// Initialize ImGui.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();

	ImFont* font = io.Fonts->AddFontFromFileTTF("Fonts/DroidSans.ttf", 32.0f);
	IM_ASSERT(font != nullptr);

	ImGui_ImplGlfw_InitForOpenGL(window->GetGLFWWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
}

void Gui::Update(double deltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (showDemoWindow)
		ImGui::ShowDemoWindow(&showDemoWindow);

	if (showLoadedModels)
		ShowLoadedModels();

	ImGui::Begin("Settings", nullptr);

	ImGui::Checkbox("Show Demo Window", &showDemoWindow);
	ImGui::Checkbox("Show Loaded Models", &showLoadedModels);

	DrawOpenFileButton();
	DrawRendererSettings();
	DrawCameraSettings();
	DrawSceneSettings();
	DrawLightSettings();
	DrawModelSettings();

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	renderer->ApplySettings(rendererSettings);
}

void Gui::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::SetRenderer(Renderer* renderer)
{
	this->renderer = renderer;
}

void Gui::DrawOpenFileButton()
{
	if (ImGui::Button("Open Model File"))
	{
		std::string fileName = OpenFileDialog("Wavefront (*.obj)\0*.obj\0FBX (*.fbx)\0*.fbx\0");
		if (fileName == "")
			std::cout << "File name is empty." << std::endl;
		else
			std::cout << "File name: " << fileName << std::endl;

		if (!fileName.empty())
			renderer->SetModel(fileName.c_str());
	}
}

void Gui::DrawRendererSettings()
{
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Checkbox("Enable Wireframe Mode", &rendererSettings.enableWireframeMode);
	ImGui::Checkbox("Is Front Face Order CCW", &rendererSettings.isFrontFaceOrderCCW);
	ImGui::Checkbox("Enable Depth Testing", &rendererSettings.enableDepthTesting);
	ImGui::Checkbox("Enable Face Culling", &rendererSettings.enableFaceCulling);
	if (rendererSettings.enableFaceCulling)
		ImGui::Checkbox("Is Culled Face Back", &rendererSettings.isCulledFaceBack);
}

void Gui::DrawCameraSettings()
{
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::DragFloat("Camera Move Speed", &renderer->cameraMoveSpeed, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("Camera Rotate Speed", &renderer->cameraRotateSpeed, 1.0f, 0.0f, 360.0f);
}

void Gui::DrawSceneSettings()
{
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::ColorEdit3("Background Color", &renderer->backgroundColor.x);
	ImGui::Checkbox("Draw Skybox", &renderer->shouldDrawSkybox);
	ImGui::Checkbox("Draw Grid", &renderer->shouldDrawGrid);
}

void Gui::DrawLightSettings()
{
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::DragFloat3("Light Direction", &renderer->directionalLight.direction.x);
	ImGui::ColorEdit3("Light Ambient Color", &renderer->directionalLight.ambient.x);
	ImGui::ColorEdit3("Light Diffuse Color", &renderer->directionalLight.diffuse.x);
	ImGui::ColorEdit3("Light Specular Color", &renderer->directionalLight.specular.x);
}

void Gui::DrawModelSettings()
{
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Checkbox("Draw Cube", &renderer->shouldDrawCube);
	if (renderer->shouldDrawCube)
	{
		ImGui::DragFloat3("Cube Position", &renderer->cubePosition.x, 0.1f);
		ImGui::DragFloat3("Cube Rotation", &renderer->cubeRotation.x, 1.0f, -180.0f, 180.0f);
		ImGui::DragFloat3("Cube Scale", &renderer->cubeScale.x, 0.1f);
		ImGui::ColorEdit3("Cube Color", &renderer->cubeColor.x);
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Checkbox("Draw Model", &renderer->shouldDrawModel);
	if (renderer->shouldDrawModel)
	{
		ImGui::DragFloat3("Model Position", &renderer->modelPosition.x, 0.1f);
		ImGui::DragFloat3("Model Rotation", &renderer->modelRotation.x, 1.0f, -180.0f, 180.0f);
		ImGui::DragFloat3("Model Scale", &renderer->modelScale.x, 0.1f);
		ImGui::DragFloat("Model Ambient Coefficient", &renderer->modelAmbientCoefficient, 0.1f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Model Diffuse Color", &renderer->modelDiffuseColor.x);
		ImGui::DragFloat("Model Diffuse Coefficient", &renderer->modelDiffuseCoefficient, 0.1f, 0.0f, 1.0f);
		ImGui::ColorEdit3("Model Specular Color", &renderer->modelSpecularColor.x);
		ImGui::DragFloat("Model Specular Coefficient", &renderer->modelSpecularCoefficient, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Model Shininess", &renderer->modelShiniess);
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Checkbox("Draw Model 2", &renderer->shouldDrawModel2);
	if (renderer->shouldDrawModel2)
	{
		ImGui::DragFloat3("Model 2 Position", &renderer->model2Position.x, 0.1f);
		ImGui::DragFloat3("Model 2 Rotation", &renderer->model2Rotation.x, 1.0f, -180.0f, 180.0f);
		ImGui::DragFloat3("Model 2 Scale", &renderer->model2Scale.x, 0.1f);
		ImGui::DragFloat("Model 2 Ambient Coefficient", &renderer->model2AmbientCoefficient, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Model 2 Diffuse Coefficient", &renderer->model2DiffuseCoefficient, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Model 2 Specular Coefficient", &renderer->model2SpecularCoefficient, 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("Model 2 Shininess", &renderer->model2Shiniess);
	}
}

void Gui::ShowLoadedModels()
{
	ImGui::Begin("Loaded Models", nullptr);

	if (ImGui::Button("Load Model File"))
	{
		std::string fileName = OpenFileDialog("Wavefront (*.obj)\0*.obj\0FBX (*.fbx)\0*.fbx\0");
		if (fileName == "")
			std::cout << "File name is empty." << std::endl;
		else
			std::cout << "File name: " << fileName << std::endl;

		if (!fileName.empty())
			renderer->LoadModel(fileName.c_str());
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Text("Loaded Models:");
	std::vector<std::shared_ptr<Model>> models = renderer->GetLoadedModels();
	for (int i = 0; i < models.size(); ++i)
	{
		std::filesystem::path modelFilePath = models[i]->GetFilePath();
		ImGui::Text(modelFilePath.generic_string().c_str());

		ImGui::SameLine();
		if (ImGui::Button(std::format("Create Model Instance##{}", i).c_str()))
			renderer->CreateModelInstance(i);

		ImGui::SameLine();
		if (ImGui::Button(std::format("Unload Model##{}", i).c_str()))
			renderer->RemoveModelAt(i);
	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::Text("Model Instances:");
	std::vector<std::shared_ptr<ModelInstance>> modelInstances = renderer->GetModelInstances();
	for (int i = 0; i < modelInstances.size(); ++i)
	{
		if (auto model = modelInstances[i]->GetModel().lock())
		{
			std::filesystem::path modelFilePath = model->GetFilePath();
			ImGui::Text(modelFilePath.generic_string().c_str());

			// Position
			glm::vec3 position = modelInstances[i]->transform.GetPosition();

			std::string positionParam = std::format("Position##{}", i);
			ImGui::DragFloat3(positionParam.c_str(), &position.x, 0.1f);

			modelInstances[i]->transform.SetPosition(position);

			// Rotation
			glm::vec3 eulerAngles = glm::eulerAngles(modelInstances[i]->transform.GetRotation());
			eulerAngles.x = glm::degrees(eulerAngles.x);
			eulerAngles.y = glm::degrees(eulerAngles.y);
			eulerAngles.z = glm::degrees(eulerAngles.z);

			std::string rotationParam = std::format("Rotation##%{}", i);
			ImGui::DragFloat3(rotationParam.c_str(), &eulerAngles.x, 1.0f, 0.0f, 360.0f);

			eulerAngles.x = glm::radians(eulerAngles.x);
			eulerAngles.y = glm::radians(eulerAngles.y);
			eulerAngles.z = glm::radians(eulerAngles.z);

			modelInstances[i]->transform.SetRotation(glm::quat(eulerAngles));

			// Scale
			glm::vec3 scale = modelInstances[i]->transform.GetScale();

			std::string scaleParam = std::format("Scale##%{}", i);
			ImGui::DragFloat3(scaleParam.c_str(), &scale.x, 0.1f);

			modelInstances[i]->transform.SetScale(scale);

			// Button to remove model instance.
			if (ImGui::Button(std::format("Remove Model Instance##{}", i).c_str()))
				renderer->RemoveModelInstance(i);

			ImGui::Dummy(ImVec2(0.0f, 20.0f));
		}
	}

	ImGui::End();
}
