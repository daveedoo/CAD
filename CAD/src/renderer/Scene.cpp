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
#include "systems/BezierC0System.h"
#include "gui/MainMenuBar.h"
#include "systems/SortingSystem.h"
#include "systems/ScreenPositionSystem.h"
#include "objects\Components\Position.h"
#include "objects\Components\Cursor.h"
#include "systems/MouseSelectionSystem.h"
#include "..\Window\input\events\modded\KeyEvent.h"
#include "commands\ApplyGroupTransformation.h"


Scene::Scene(unsigned int frame_width, unsigned int frame_height, std::shared_ptr<Window> window) :
	window(window),
	camera(std::make_shared<Camera>(90, frame_width, frame_height, 0.1f, 100.f)),
	cameraMovementHandler(std::make_shared<CameraMovementInputHandler>(*this->camera)),
	floor(std::make_unique<Floor>(50, 50)),
	registry(std::make_shared<entt::registry>()),
	curveSegmentsMetrics(std::make_shared<BernsteinPolygonMetrics>(camera, frame_width, frame_height)),
	mouseSelectionSystem(std::make_shared<MouseSelectionSystem>(registry, camera, window))
{
	auto transformationsSystem = std::make_shared<TransformationsSystem>(registry);
	entitiesFactory = std::make_shared<EntitiesFactory>(this->registry);	// after transformationsSystem
	selectionSystem = std::make_shared<SelectionSystem>(registry, entitiesFactory);

	// build GUI
	auto mainMenuBar = std::make_unique<MainMenuBar>(*this, registry);
	auto guiSystem = std::make_shared<GUISystem>(registry, std::move(mainMenuBar), *this);

	auto groupTransformation = std::make_shared<AdditionalTransformation>(glm::vec3(0.f), 1.f);
	auto start = std::make_shared<StartGroupTransformation>(registry, selectionSystem, groupTransformation);
	auto change = std::make_shared<ChangeGroupTransformation>(registry, selectionSystem, groupTransformation);
	auto apply = std::make_shared<ApplyGroupTransformation>(registry, groupTransformation);
	auto cancel = std::make_shared<CancelGroupTransformation>(registry, groupTransformation);
	auto groupTransformationGUI = std::make_unique<GroupTransformationWindow>(groupTransformation, start, change, apply, cancel);
	guiSystem->AddGroupWindow(std::move(groupTransformationGUI));

	// systems
	auto bezierC0System = std::make_shared<BezierC0System>(registry, cameraMovementHandler, curveSegmentsMetrics);
	auto screenPositionSystem = std::make_shared<ScreenPositionSystem>(registry, *camera);
	this->AddSubscriber(screenPositionSystem);

	this->systems.push_back(std::make_shared<PointSystem>(registry));
	this->systems.push_back(std::make_shared<SortingSystem>(registry));
	this->systems.push_back(std::make_shared<TorusSystem>(registry));
	this->systems.push_back(std::make_shared<PointSystem>(registry));
	this->systems.push_back(std::make_shared<CursorSystem>(registry));
	this->systems.push_back(transformationsSystem);
	this->systems.push_back(screenPositionSystem);
	this->systems.push_back(bezierC0System);
	this->systems.push_back(guiSystem);
	this->systems.push_back(this->selectionSystem);

	// camera and movement
	this->camera->Scale(1.f / 10.f);
	this->cameraMovementHandler->AddSubscriber(bezierC0System);
	this->cameraMovementHandler->AddSubscriber(screenPositionSystem);


	// create starting scene entities
	this->mainCursor = entitiesFactory->CreateCursor(glm::vec3(0.f), 3.f, 1.f);

	const auto& point1 = this->entitiesFactory->CreatePoint(0.f, 1.f, 0.f);
	const auto& point2 = this->entitiesFactory->CreatePoint(1.f, 7.f, 1.f);
	const auto& point3 = this->entitiesFactory->CreatePoint(2.f, 3.f, 3.f);
	const auto& point4 = this->entitiesFactory->CreatePoint(3.f, 4.f, 8.f);

	this->entitiesFactory->CreateTorus(1.f, 5.f, 10, 10, glm::vec3(0.f, 12.f, -2.f));

	//auto bezierPoints = std::vector<entt::entity>{
	//	point1, point2, point3, point4
	//};
	//this->entitiesFactory->CreateBezierC0(bezierPoints);
}

void Scene::HandleEvent(const InputEvent& inputEvent)	// TODO: change event type to be not ResizeEvent
{
	if (inputEvent.type == InputEvent::EventType::KEY)
	{
		const auto& keyEvent = static_cast<const KeyEvent&>(inputEvent);
		if (keyEvent.key == GLFW_KEY_LEFT_CONTROL)
		{
			if (keyEvent.action == KeyOrButtonEvent::Action::PRESS)
				isCtrlDown = true;
			else if (keyEvent.action == KeyOrButtonEvent::Action::RELEASE)
			{
				isCtrlDown = false;
				this->window->SetCursor(Window::CursorType::Normal);
			}
		}
	}

	if (isCtrlDown)
		this->mouseSelectionSystem->ProcessInput(inputEvent);
	else
		this->cameraMovementHandler->ProcessInput(inputEvent);
}

void Scene::SetFramebufferSize(unsigned int width, unsigned int height)
{
	this->camera->SetViewportSize(width, height);
	this->curveSegmentsMetrics->UpdateScreenSize(width, height);
	
	this->NotifySubscribers(width, height);
}

void Scene::Update()
{
	for (auto& system : this->systems)
	{
		system->Update(*this->camera);
	}
}

void Scene::Render()
{
	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: make it to ECS
	this->floor->Render(*this->camera);

	for (auto& system : this->systems)
	{
		system->Render(*this->camera);
	}
}


entt::entity Scene::AddTorus()
{
	auto& cursorPos = this->registry->get<Position>(this->mainCursor).position;
	return entitiesFactory->CreateTorus(1.f, 5.f, 10, 10, cursorPos);
}

entt::entity Scene::AddPoint()
{
	auto& cursorPos = this->registry->get<Position>(this->mainCursor).position;
	return entitiesFactory->CreatePoint(cursorPos);
}

entt::entity Scene::AddBezierC0(const std::vector<entt::entity>& points)
{
	return entitiesFactory->CreateBezierC0(points);
}

void Scene::RemoveEntity(entt::entity entity)
{
	this->registry->destroy(entity);
}
