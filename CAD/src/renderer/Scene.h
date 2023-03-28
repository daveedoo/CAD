#pragma once
#include <memory>
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VAO.h"
#include "../Camera.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "objects/Floor/Floor.h"
#include "objects/Component.h"
#include <entt/entt.hpp>
#include "GUI.h"
#include "ObjectsManager.h"

class Scene
{
private:
	const glm::vec3 bgColor = glm::vec3(0.4f, 0.4f, 0.4f);

	std::unique_ptr<Camera> camera;
	std::unique_ptr<CameraMovementInputHandler> cameraMovementHandler;

	std::unique_ptr<Floor> floor;

	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<ObjectsManager> objectsManager;

	std::unique_ptr<GL::Program> torusProgram;
	std::unique_ptr<GL::Program> cursorProgram;

	std::unique_ptr<GL::VAO> pointsVao;

	entt::entity cursor;
	std::unique_ptr<GUI> gui;

public:
	Scene(unsigned int frame_width, unsigned int frame_height);

	void Update();
	void Render();

	void HandleEvent(const InputEvent& inputEvent);
	bool IsSceneMoving() const { return cameraMovementHandler->IsCameraMoving(); }
	void SetFramebufferSize(unsigned int width, unsigned int height);

private:
	void transformations_system();
	void torus_system();
	void namedEntities_system();
	void cursors_system();
	void points_system();
};
