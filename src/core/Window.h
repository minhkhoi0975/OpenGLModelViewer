#pragma once
#include "Delegate.h"

class Window
{
public:
	void Init(int width, int height, const char* title);
	void Shutdown();

public:
	void GetWindowSize(int* width, int* height);
	void GetCursorPosition(double* cursorPositionX, double* cursorPositionY);

	// Set this window as context for renderer.
	void MakeContextCurrent();

	double GetCurrentTime();

	int ShouldClose();
	void PollEvents();
	void SwapBuffers();

	void CloseWindow();

public:
	// Events.
	virtual void OnWindowResized(int newWidth, int newHeight) {}

	virtual void OnKeyPressed(int key) {}
	virtual void OnKeyReleased(int key) {}
	virtual void OnKeyHeld(int key) {}

	virtual void OnMousePressed(int button) {}
	virtual void OnMouseReleased(int button) {}
	virtual void OnCursorMove(double newPositionX, double newPositionY) {};

	MulticastDelegate<int, int> onWindowResized;		// int: new width, int: new height

	MulticastDelegate<int> onKeyPressed;				// int: key code
	MulticastDelegate<int> onKeyReleased;				// int: key code
	MulticastDelegate<int> onKeyHeld;					// int: key code

	MulticastDelegate<int> onMousePressed;				// int: button code
	MulticastDelegate<int> onMouseReleased;				// int: button code
	MulticastDelegate<double, double> onCursorMoved;	// double: new position X, double: new position y

public:
	struct GLFWwindow* GetGLFWWindow();

private:
	struct GLFWwindow* glfwWindow;
};