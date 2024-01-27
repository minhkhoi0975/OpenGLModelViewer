#include "Application.h"
#include "functional"

Application::Application(char** argv, int argc)
{
	resourceManager.Init();

	window.Init(800, 640, "OpenGL");
	window.MakeContextCurrent();

	renderer.Init(&window);

	gui.Init(&window);
	gui.SetRenderer(&renderer);
}

Application::~Application()
{
	gui.Shutdown();
	
	renderer.Shutdown();
	
	window.Shutdown();

	resourceManager.Shutdown();
}

void Application::Run()
{
	while (!window.ShouldClose())
	{
		double latestTime = window.GetCurrentTime();
		deltaTime = latestTime - currentTime;
		currentTime = latestTime;

		renderer.Update(deltaTime);

		gui.Update(deltaTime);
	
		window.PollEvents();
		window.SwapBuffers();
	}
}

double Application::GetCurrentTime() const
{
	return currentTime;
}

double Application::GetDeltaTime() const
{
	return deltaTime;
}
