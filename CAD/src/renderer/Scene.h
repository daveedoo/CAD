#pragma once
#include <memory>
#include <vector>
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VAO.h"
#include "../Camera.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "objects/Floor/Floor.h"
#include <entt/entt.hpp>
#include "EntitiesFactory.h"
#include "../ScreenSizeChangePublisher.h"
#include "systems/System.h"
#include "systems/GUISystem.h"
#include "systems/TransformationsSystem.h"
#include "systems/SelectionSystem.h"
#include "systems/BezierC0System.h"
#include "systems/CurveSegmentsMetrics/BernsteinPolygonMetrics.h"
#include "systems/MouseSelectionSystem.h"
#include "..\Window\Window.h"

class Scene : public ScreenSizeChangePublisher
{
private:
	const GLfloat SelectedObjectCursor_LineWidth = 2.f;
	const float SelectedObjectCursor_LineLength = 0.5f;
	const glm::vec3 bgColor = glm::vec3(0.4f, 0.4f, 0.4f);

	std::shared_ptr<Window> window;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<CameraMovementInputHandler> cameraMovementHandler;
	bool isCtrlDown = false;
	std::unique_ptr<Floor> floor;

	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<EntitiesFactory> entitiesFactory;
	std::shared_ptr<BernsteinPolygonMetrics> curveSegmentsMetrics;

	entt::entity mainCursor;

	std::vector<std::shared_ptr<System>> systems;
	std::shared_ptr<MouseSelectionSystem> mouseSelectionSystem;
	std::shared_ptr<SelectionSystem> selectionSystem;

public:
	Scene(unsigned int frame_width, unsigned int frame_height, std::shared_ptr<Window> window);

	void Update();
	void Render();

	void HandleEvent(const InputEvent& inputEvent);
	bool IsSceneMoving() const { return cameraMovementHandler->IsCameraMoving(); }
	void SetFramebufferSize(unsigned int width, unsigned int height);
	entt::entity GetMainCursor() { return this->mainCursor; }

	entt::entity AddTorus();
	entt::entity AddPoint();
	entt::entity AddBezierC0(const std::vector<entt::entity>& points);
	void RemoveEntity(entt::entity entity);
};
