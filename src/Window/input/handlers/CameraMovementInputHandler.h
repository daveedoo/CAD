#pragma once
#include "../../Camera.h"
#include "../events/InputEvent.h"
#include "../events/MouseMoveEvent.h"
#include "../events/modded/MouseClickEvent.h"
#include "InputHandler.h"

class CameraMovementInputHandler : public InputHandler
{
private:
	Camera& camera;
	bool rotationOn = false;

	glm::dvec2 lastMousePos = glm::dvec2(0.0, 0.0);
	bool firstMouseEventFired = false;

	void HandleMouseClickEvent(const MouseClickEvent& event);
	void HandleMouseMoveEvent(const MouseMoveEvent& event);

public:
	CameraMovementInputHandler(Camera& camera) :
		camera(camera) {}

	void ProcessInput(const InputEvent& event) override;
};
