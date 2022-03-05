#pragma once
#include <GLFW/glfw3.h>
#include <string>

class Window
{
private:
	GLFWwindow* window;

public:
	Window(int width, int height, std::string title);
	~Window();

	bool ShouldClose();
	void PollEvents();
	void SwapBuffers();
};
