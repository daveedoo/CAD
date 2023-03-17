#include "Scene.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Config.h"
#include <filesystem>
#include "../gl/wrappers/Shader.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "../Window/input/events/ResizeEvent.h"

const glm::vec3 Scene::bgColor = glm::vec3(0.4f, 0.4f, 0.4f);

Scene::Scene() :
	camera(std::make_unique<Camera>(90, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.f)),	// TODO: change
	cameraMovementHandler(std::make_unique<CameraMovementInputHandler>(*this->camera)),
	ellipsoid(std::make_unique<Ellipsoid>(0.01f, 0.2f, 3.f)),
	torus(std::make_unique<Torus>(0.1f, 0.5f, 8, 8))
{ }

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->ellipsoid->SetFramebufferSize(width, height);
	this->camera->SetAspect((float)width / (float)height);
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
}
