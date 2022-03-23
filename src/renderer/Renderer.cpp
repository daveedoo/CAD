#include <gl/glew.h>
#include <exception>
#include <glm/gtc/matrix_transform.hpp>

#include <Config.h>
#include "Renderer.h"
#include "../gl/wrappers/VAO.h"
#include "../gl/wrappers/Shader.h"
#include "../gl/wrappers/Program.h"
#include "../Window/Window.h"

Renderer::Renderer(Window& window) : window(window)
{
	this->window.MakeContextCurrent();

	if (glewInit() != GLEW_OK)
		throw std::exception("glewInit failed");
	this->UpdateViewport();

	window.SetEventHandler([&](const InputEvent& event)
		{
			this->HandleEvent(event);
		});

	this->gui = std::make_unique<GUI>(window);
	this->scene = std::make_unique<Scene>();
}

void Renderer::UpdateViewport()
{
	int width, height;
	window.GetFramebufferSize(width, height);
	glViewport(0, 0, width, height);
}

void Renderer::HandleEvent(const InputEvent& inputEvent)
{
	if (inputEvent.type == InputEvent::EventType::RESIZE)
		this->UpdateViewport();
	if (!this->gui->WantCaptureMouse())
		this->scene->HandleEvent(inputEvent);
}

void Renderer::Draw()
{
	this->scene->Draw(gui->GetCoefficients());
	this->gui->Draw();
}
