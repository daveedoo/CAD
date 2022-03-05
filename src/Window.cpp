#include "Window.h"
#include "Config.h"

#include <exception>

Window::Window(int width, int height, std::string title)
{
	if (!glfwInit())
		throw std::exception("Initialization failed");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (this->window == NULL)
		throw std::exception("Window or context creation failed");

	glfwMakeContextCurrent(this->window);
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(this->window) != 0;
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(this->window);
}
