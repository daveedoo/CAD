#include "Window.h"
#include "../Config.h"

#include <exception>
#include <glm/glm.hpp>
#include "input/events/modded/MouseClickEvent.h"
#include "input/events/MouseMoveEvent.h"
#include "input/events/modded/KeyEvent.h"
#include "input/events/MouseScrollEvent.h"
#include "input/events/ResizeEvent.h"
#include <iostream>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600

unsigned int Window::windowsCounter = 0;

Window::Window(int width, int height, std::string title)
{
	if (windowsCounter == 0)
	{
		if (!glfwInit())
			throw std::exception("Initialization failed");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (this->window == NULL)
		throw std::exception("Window or context creation failed");
	Window::windowsCounter++;

	glfwSetErrorCallback([](int error, const char* description)
		{
			std::cerr << std::format("GLFW Error {}: {}\n", error, description);
		});
	glfwSetWindowSizeLimits(this->window, MIN_WIDTH, MIN_HEIGHT, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	windowsCounter--;

	if (windowsCounter == 0)
		glfwTerminate();
}

void Window::GetFramebufferSize(int& width, int& height) const
{
	glfwGetFramebufferSize(this->window, &width, &height);
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(this->window);
}

void Window::SetFramebufferSizeEventHandler(std::function<void(const ResizeEvent&)> callback) {
	glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow* window, int width, int height)
		{
			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
			const ResizeEvent event = ResizeEvent(width, height);
			thisWindow->onFramebufferSizeEvent(event);
		});
	this->onFramebufferSizeEvent = callback;
}

void Window::SetInputEventHandler(std::function<void(const InputEvent&)> callback)
{
	glfwSetWindowUserPointer(this->window, this);

	// only the used events are handled
	glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
			const MouseClickEvent inputEvent = MouseClickEvent(button, action, mods);
			thisWindow->onInputEvent(inputEvent);
		});
	glfwSetCursorPosCallback(this->window, [](GLFWwindow* window, double xpos, double ypos)
		{
			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
			const MouseMoveEvent inputEvent = MouseMoveEvent(xpos, ypos);
			thisWindow->onInputEvent(inputEvent);
		});
	glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
			const KeyEvent inputEvent = KeyEvent(key, scancode, action, mods);
			thisWindow->onInputEvent(inputEvent);
		});
	glfwSetScrollCallback(this->window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
			const MouseScrollEvent inputEvent = MouseScrollEvent(xoffset, yoffset);
			thisWindow->onInputEvent(inputEvent);
		});

	this->onInputEvent = callback;
}

bool Window::ShouldClose() const
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
