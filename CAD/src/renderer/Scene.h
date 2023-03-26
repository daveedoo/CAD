#pragma once
#include <memory>
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VAO.h"
#include "../Camera.h"
#include "../Window/input/handlers/CameraMovementInputHandler.h"
#include "objects/Ellipsoid/Ellipsoid.h"
#include "objects/Floor/Floor.h"
#include "objects/Cursos/Cursor.h"
#include "objects/Component.h"
#include <entt/entt.hpp>

class Scene
{
private:
	const glm::vec3 bgColor = glm::vec3(0.4f, 0.4f, 0.4f);

	std::unique_ptr<Camera> camera;
	std::unique_ptr<CameraMovementInputHandler> cameraMovementHandler;

	std::unique_ptr<Floor> floor;

	std::unique_ptr<entt::registry> registry;
	std::unique_ptr<GL::Program> torusProgram;


public:
	std::unique_ptr<Cursor> cursor;

	Scene(unsigned int frame_width, unsigned int frame_height);

	void Update();
	void Render();

	void HandleEvent(const InputEvent& inputEvent);
	bool IsSceneMoving() const { return cameraMovementHandler->IsCameraMoving(); }
	void SetFramebufferSize(unsigned int width, unsigned int height);

	void CreateTorus(float minorR, float majorR, int minorSegments, int majorSegments);

private:
	void torus_system();
	void namedEntities_system();
};
