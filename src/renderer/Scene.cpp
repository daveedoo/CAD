#include "Scene.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Config.h>
#include <filesystem>
#include "../gl/wrappers/Shader.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "../Window/input/events/ResizeEvent.h"

Scene::Scene() :
	camera(std::make_unique<Camera>()),
	elipsoide(std::make_unique<Elipsoide>())
{
	this->cameraMovementHandler = std::make_unique<CameraMovementInputHandler>(*this->camera);
}

void Scene::HandleEvent(const InputEvent& inputEvent)
{
	if (inputEvent.type == InputEvent::EventType::RESIZE)
	{
		const ResizeEvent& resizeEvent = static_cast<const ResizeEvent&>(inputEvent);
		this->elipsoide->SetFramebufferSize(resizeEvent.width, resizeEvent.height);
	}

	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::Draw(Coefficients& coeff)
{
	glClearColor(1.0f, 0.66f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	elipsoide->SetA(coeff.A);
	elipsoide->SetB(coeff.B);
	elipsoide->SetC(coeff.C);
	elipsoide->Draw(*this->camera);
}
