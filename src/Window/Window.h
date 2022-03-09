#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include "input/InputHandler.h"

class Window
{
private:
	GLFWwindow* window;
	std::vector<std::unique_ptr<InputHandler>> inputHandlers;

public:
	Window(int width, int height, std::string title);
	~Window();

	void SubscribeInputHandler(std::unique_ptr<InputHandler> inputHandler);

	bool ShouldClose();
	void PollEvents();
	void SwapBuffers();
};
