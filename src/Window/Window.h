#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include "input/handlers/InputHandler.h"
#include <functional>

class Window
{
private:
	static unsigned int windowsCounter;

	GLFWwindow* window;
	std::function<void(const InputEvent& event)> onEvent = {};

public:
	Window(int width, int height, std::string title, int minWidth = 800, int minHeight = 800);
	~Window();

	GLFWwindow* GetRawWindow() { return this->window; }
	void GetFramebufferSize(int& width, int& height) const;

	void MakeContextCurrent();
	void SetEventHandler(std::function<void(const InputEvent& event)> callback);

	bool ShouldClose() const;
	void PollEvents();
	void SwapBuffers();
};
