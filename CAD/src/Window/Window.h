#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include "input/handlers/InputHandler.h"
#include "input/events/ResizeEvent.h"
#include <functional>

/// <summary>
/// Wrapper around the GLFW window.
/// </summary>
class Window
{
public:
	enum CursorType
	{
		Normal,
		Hand
	};

private:
	static unsigned int windowsCounter;

	GLFWwindow* window;
	GLFWcursor* handCursor;
	CursorType currentCursor = Normal;
	std::function<void(const InputEvent&)> onInputEvent = {};
	std::function<void(const ResizeEvent&)> onFramebufferSizeEvent = {};

public:
	Window(int width, int height, std::string title);
	~Window();

	GLFWwindow* GetRawWindow() { return this->window; }
	void GetFramebufferSize(int& width, int& height) const;

	void MakeContextCurrent();
	/// <summary>
	/// Sets the callback to be fired on every key / mouse event.
	/// </summary>
	/// <param name="callback"></param>
	void SetFramebufferSizeEventHandler(std::function<void(const ResizeEvent&)> callback);
	void SetInputEventHandler(std::function<void(const InputEvent&)> callback);
	void SetCursor(CursorType type);
	CursorType GetCursorType() { return this->currentCursor; }

	bool ShouldClose() const;
	void PollEvents();
	void SwapBuffers();
};
