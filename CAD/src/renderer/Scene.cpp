#include "Scene.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Config.h"
#include <filesystem>
#include "../gl/wrappers/Shader.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "../Window/input/events/ResizeEvent.h"


Scene::Scene(unsigned int frame_width, unsigned int frame_height) :
	camera(std::make_unique<Camera>(90, static_cast<float>(frame_width) / static_cast<float>(frame_height), 0.1f, 100.f)),
	cameraMovementHandler(std::make_unique<CameraMovementInputHandler>(*this->camera)),
	//ellipsoid(std::make_unique<Ellipsoid>(0.01f, 0.2f, 3.f)),
	torus(std::make_unique<Torus>(0.1f, 0.5f, 8, 8))
{
	this->floor = std::make_unique<Floor>(50, 50);
}

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	//this->ellipsoid->SetFramebufferSize(width, height);
	this->camera->SetAspect(static_cast<float>(width) / static_cast<float>(height));
}

void Scene::Update()
{
}

void Scene::Render()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//this->ellipsoid->Render(*this->camera);
	this->torus->Render(*this->camera);
	this->floor->Render(*this->camera);
}
