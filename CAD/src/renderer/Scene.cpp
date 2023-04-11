#include "Scene.h"
#include <gl/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "../Config.h"
#include <filesystem>
#include "../gl/wrappers/Shader.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "../Window/input/events/ResizeEvent.h"
#include <imgui.h>
#include <glm/gtx/rotate_vector.hpp>
#include <imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>
#include "../../Matrix.h"
#include "../gl/ProgramFactory.h"
#include "systems/TorusSystem.h"
#include "systems/PointSystem.h"
#include "systems/CursorSystem.h"
#include "systems/GUISystem.h"
#include "commands/Command.h"
#include "commands/StartGroupTransformation.h"
#include "gui/GroupTransformationWindow.h"
#include "commands/ChangeGroupTransformation.h"
#include "commands/CancelGroupTransformation.h"


Scene::Scene(unsigned int frame_width, unsigned int frame_height) :
	camera(std::make_unique<Camera>(90, static_cast<float>(frame_width) / static_cast<float>(frame_height), 0.1f, 100.f)),
	cameraMovementHandler(std::make_unique<CameraMovementInputHandler>(*this->camera)),
	floor(std::make_unique<Floor>(50, 50)),
	registry(std::make_shared<entt::registry>()),
	objectsManager(std::make_shared<EntitiesFactory>(this->registry)),
	//gui(std::make_shared<GUI>(*this, this->objectsManager)),
	torusSystem(std::make_unique<TorusSystem>(registry)),
	pointsSystem(std::make_unique<PointSystem>(registry)),
	cursorSystem(std::make_unique<CursorSystem>(registry)),
	guiSystem(std::make_unique<GUISystem>(registry, *this)),
	transformationsSystem(std::make_unique<TransformationsSystem>(registry)),
	selectionSystem(std::make_shared<SelectionSystem>(registry, objectsManager)),
	mainCursor(objectsManager->CreateCursor(glm::vec3(0.f), 3.f, 1.f))
{
	this->camera->Scale(1.f / 10.f);

	// build GUI
	auto groupScaleRoation = std::make_shared<ScaleRotation>();
	auto start = std::make_shared<StartGroupTransformation>(registry, selectionSystem, groupScaleRoation);
	auto change = std::make_shared<ChangeGroupTransformation>(registry, selectionSystem, groupScaleRoation);
	auto cancel = std::make_shared<CancelGroupTransformation>(registry, groupScaleRoation);
	auto groupTransformationGUI = std::make_unique<GroupTransformationWindow>(groupScaleRoation, start, change, cancel, cancel);
	this->guiSystem->AddGroupWindow(std::move(groupTransformationGUI));


	//this->objectsManager->CreateTorus(1.f, 3.f, 10, 10, glm::vec3(0.f));
	const auto& point1 = this->objectsManager->CreatePoint(0.f, 1.f, 0.f);
	const auto& point2 = this->objectsManager->CreatePoint(1.f, 2.f, 1.f);
	//this->objectsManager->CreateTorus(1.f, 10.f, 20, 20, glm::vec3(0.f));

	auto bezierPoints = std::vector<entt::entity>{
		point1, point2
	};
	this->objectsManager->CreateBezierC0(bezierPoints);
}

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->camera->SetAspect(static_cast<float>(width) / static_cast<float>(height));
}

void Scene::Update()
{
	this->selectionSystem->Update(*this->camera);
	this->transformationsSystem->Update(*this->camera);
}

void Scene::Render()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: make it to ECS
	this->floor->Render(*this->camera);

	this->guiSystem->Render(*this->camera);
	this->torusSystem->Render(*this->camera);
	this->pointsSystem->Render(*this->camera);
	this->cursorSystem->Render(*this->camera);
}


entt::entity Scene::AddTorus()
{
	auto& cursorPos = this->registry->get<Position>(this->mainCursor).position;
	return objectsManager->CreateTorus(1.f, 5.f, 10, 10, cursorPos);
}

entt::entity Scene::AddPoint()
{
	auto& cursorPos = this->registry->get<Position>(this->mainCursor).position;
	return objectsManager->CreatePoint(cursorPos);
}

void Scene::RemoveEntity(entt::entity entity)
{
	this->registry->destroy(entity);
}

void Scene::SetSelected(entt::entity entity)
{
	this->registry->emplace_or_replace<Cursor>(entity, SelectedObjectCursor_LineWidth, SelectedObjectCursor_LineLength);
	this->selectionSystem->UpdateCursor();
}

void Scene::SetUnselected(entt::entity entity)
{
	this->registry->remove<Cursor>(entity);
	this->selectionSystem->UpdateCursor();
}
