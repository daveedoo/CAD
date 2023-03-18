#pragma once
#include "../../../Camera.h"
#include "../events/InputEvent.h"
#include "../events/MouseMoveEvent.h"
#include "../events/MouseScrollEvent.h"
#include "../events/modded/MouseClickEvent.h"
#include "InputHandler.h"
#include <optional>

class CameraMovementInputHandler : public InputHandler
{
private:
	Camera& camera;
	bool rotationOn = false;
	bool translationOn = false;

	std::optional<glm::dvec2> lastMousePos = std::nullopt;

	void HandleMouseClickEvent(const MouseClickEvent& event);
	void HandleMouseMoveEvent(const MouseMoveEvent& event);
	void HandleScrollEvent(const MouseScrollEvent& event);

public:
	CameraMovementInputHandler(Camera& camera) :
		camera(camera) {}

	void ProcessInput(const InputEvent& event) override;
	bool IsCameraMoving() const { return rotationOn || translationOn; }
};
