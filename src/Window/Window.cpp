#include "Window.h"
#include "Config.h"
#include "input/InputEvent.h"

#include <exception>
#include <glm/glm.hpp>


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

	glfwSetWindowUserPointer(this->window, this);

	glfwSetErrorCallback([](int error, const char* description)
		{
			fprintf(stderr, "GLFW Error %d: %s\n", error, description);
		});

	//glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods)
	//		{
	//			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	//			const InputEvent inputEvent = InputEvent(key, scancode, action, mods);
	//			for (auto& inputHandler : thisWindow->inputHandlers)
	//				inputHandler->ProcessInput(inputEvent);
	//		});
	glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* thisWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

			const KeyEvent inputEvent = KeyEvent(key, scancode, action, mods);
			for (auto& inputHandler : thisWindow->inputHandlers)
				inputHandler->ProcessInput(inputEvent);
		});
}

Window::~Window()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void Window::SubscribeInputHandler(std::unique_ptr<InputHandler> inputHandler)
{
	inputHandlers.push_back(std::move(inputHandler));
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
