#include "Window.h"

#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>

void Window::Init(int width, int height, const char* title)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Error: Cannot initialize GLFW." << std::endl;
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	// Use OpenGL 3.3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a window.
	glfwWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!glfwWindow)
	{
		std::cerr << "Error: Cannot create a window." << std::endl;
		glfwDestroyWindow(glfwWindow);
		std::exit(EXIT_FAILURE);
	}

	glfwSetWindowUserPointer(glfwWindow, this);

	// Bind functions to events.
	glfwSetFramebufferSizeCallback(glfwWindow, [](GLFWwindow* glfwWindow, int width, int height)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
		window->OnWindowResized(width, height);
		window->onWindowResized.Broadcast(width, height);
	});

	glfwSetKeyCallback(glfwWindow, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
		switch (action)
		{
		case GLFW_PRESS:
			window->OnKeyPressed(key);
			window->onKeyPressed.Broadcast(key);
			break;
		case GLFW_RELEASE:
			window->OnKeyReleased(key);
			window->onKeyReleased.Broadcast(key);
			break;
		case GLFW_REPEAT:
			window->OnKeyHeld(key);
			window->onKeyHeld.Broadcast(key);
			break;
		}
	});

	glfwSetMouseButtonCallback(glfwWindow, [](GLFWwindow* glfwWindow, int button, int action, int mods)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
		switch (action)
		{
		case GLFW_PRESS:
			window->OnMousePressed(button);
			window->onMousePressed.Broadcast(button);
			break;
		case GLFW_RELEASE:
			window->OnMouseReleased(button);
			window->onMouseReleased.Broadcast(button);
			break;
		}
	});

	glfwSetCursorPosCallback(glfwWindow, [](GLFWwindow* glfwWindow, double newMouseX, double newMouseY)
	{
		Window* window = (Window*)glfwGetWindowUserPointer(glfwWindow);
		window->OnCursorMove(newMouseX, newMouseY);
		window->onCursorMoved.Broadcast(newMouseX, newMouseY);
	});
}

void Window::Shutdown()
{
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

void Window::GetWindowSize(int* width, int* height)
{
	glfwGetWindowSize(glfwWindow, width, height);
}

void Window::GetCursorPosition(double* cursorPositionX, double* cursorPositionY)
{
	glfwGetCursorPos(glfwWindow, cursorPositionX, cursorPositionY);
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(glfwWindow);
}

double Window::GetCurrentTime()
{
	return glfwGetTime();
}

int Window::ShouldClose()
{
	return glfwWindowShouldClose(glfwWindow);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(glfwWindow);
}

void Window::CloseWindow()
{
	glfwSetWindowShouldClose(glfwWindow, 1);
}

GLFWwindow* Window::GetGLFWWindow()
{
	return glfwWindow;
}
