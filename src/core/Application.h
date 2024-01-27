#pragma once
#include "../resource_management/ResourceManager.h"
#include "Window.h"
#include "../renderer/renderer.h"
#include "../gui/gui.h"

class Application
{
public:
	Application(char** argv, int argc);
	~Application();

	void Run();

	double GetCurrentTime() const;
	double GetDeltaTime() const;

private:
	ResourceManager resourceManager;
	Window window;
	Renderer renderer;
	Gui gui;

	double currentTime = 0.0f;
	double deltaTime = 0.0f;
};