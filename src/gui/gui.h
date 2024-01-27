#pragma once

#include "../core/Window.h"

#include "../renderer/renderer.h"
#include "../renderer/RendererSettings.h"

class Gui
{
public:
	void Init(Window* window);
	void Update(double deltaTime);
	void Shutdown();

	void SetRenderer(Renderer* renderer);

private:
	void DrawOpenFileButton();
	void DrawRendererSettings();
	void DrawCameraSettings();
	void DrawSceneSettings();
	void DrawLightSettings();
	void DrawModelSettings();

	void ShowLoadedModels();

private:
	Renderer* renderer;
	RendererSettings rendererSettings;

	bool showDemoWindow = true;
	bool showLoadedModels = false;
};