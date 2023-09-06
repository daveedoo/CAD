#pragma once
#include "../../../Camera.h"
#include "../events/InputEvent.h"
#include "../events/MouseMoveEvent.h"
#include "../events/MouseScrollEvent.h"
#include "../events/modded/MouseClickEvent.h"
#include "InputHandler.h"
#include <optional>
#include "../../../CameraSubscriber.h"
#include <vector>

class CameraMovementInputHandler : public InputHandler
{
private:
	Camera& camera;
	bool rotationOn = false;
	bool translationOn = false;

	std::optional<glm::dvec2> lastMousePos = std::nullopt;
	std::vector<std::weak_ptr<CameraSubscriber>> cameraMoveSubscribers;

public:
	CameraMovementInputHandler(Camera& camera) :
		camera(camera) {}

	void ProcessInput(const InputEvent& event) override;
	bool IsCameraMoving() const { return rotationOn || translationOn; }

	void AddSubscriber(std::shared_ptr<CameraSubscriber> subscriber);

private:
	void HandleMouseClickEvent(const MouseClickEvent& event);
	void HandleMouseMoveEvent(const MouseMoveEvent& event);
	void HandleScrollEvent(const MouseScrollEvent& event);

	void NotifySubscribers();
};
