#include "Scene.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Config.h"
#include <filesystem>
#include "../gl/wrappers/Shader.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "../Window/input/events/ResizeEvent.h"

Scene::Scene() :
	camera(std::make_unique<Camera>()),
	cameraMovementHandler(std::make_unique<CameraMovementInputHandler>(*this->camera)),
	ellipsoid(std::make_unique<Ellipsoid>(1.f, 2.f, 3.f))
{
	//this->cameraMovementHandler = std::make_unique<CameraMovementInputHandler>(*this->camera);
}

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->ellipsoid->SetFramebufferSize(width, height);
}

void Scene::Update()
{
}

void Scene::Render()
{
	glClearColor(1.0f, 0.66f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ellipsoid->Render(*this->camera);
}
