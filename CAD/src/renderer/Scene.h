#pragma once
#include <memory>
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VAO.h"
#include "../Camera.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "objects/Floor/Floor.h"
#include "objects/Component.h"
#include <entt/entt.hpp>
#include "EntitiesFactory.h"
#include "systems/System.h"
#include "systems/GUISystem.h"
#include "systems/TransformationsSystem.h"
#include "systems/SelectionSystem.h"
#include "systems/BezierC0System.h"
#include "systems/CurveSegmentsMetrics/BernsteinPolygonMetrics.h"

class Scene
{
private:
	const GLfloat SelectedObjectCursor_LineWidth = 2.f;
	const float SelectedObjectCursor_LineLength = 0.5f;
	const glm::vec3 bgColor = glm::vec3(0.4f, 0.4f, 0.4f);

	std::shared_ptr<Camera> camera;
	std::shared_ptr<CameraMovementInputHandler> cameraMovementHandler;
	std::unique_ptr<Floor> floor;

	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<EntitiesFactory> entitiesFactory;
	std::shared_ptr<BernsteinPolygonMetrics> curveSegmentsMetrics;

	entt::entity mainCursor;

	std::unique_ptr<System> torusSystem;
	std::unique_ptr<System> pointsSystem;
	std::unique_ptr<System> cursorSystem;
	std::unique_ptr<GUISystem> guiSystem;
	std::unique_ptr<TransformationsSystem> transformationsSystem;
	std::shared_ptr<SelectionSystem> selectionSystem;
	std::shared_ptr<BezierC0System> bezierC0System;

public:
	Scene(unsigned int frame_width, unsigned int frame_height);

	void Update();
	void Render();

	void HandleEvent(const InputEvent& inputEvent);
	bool IsSceneMoving() const { return cameraMovementHandler->IsCameraMoving(); }
	void SetFramebufferSize(unsigned int width, unsigned int height);
	entt::entity GetMainCursor() { return this->mainCursor; }

	entt::entity AddTorus();
	entt::entity AddPoint();
	void RemoveEntity(entt::entity entity);
	void SetSelected(entt::entity entity);
	void SetUnselected(entt::entity entity);
};
