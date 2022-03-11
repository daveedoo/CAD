#include "CameraMovementInputHandler.h"

void CameraMovementInputHandler::ProcessInput(const InputEvent& e)
{
	if (e.type == InputEvent::EventType::MOUSE_CLICK)
	{
		const MouseClickEvent& event = static_cast<const MouseClickEvent&>(e);
		this->HandleMouseClickEvent(event);
	}
	else if (e.type == InputEvent::EventType::MOUSE_MOVE)
	{
		const MouseMoveEvent& event = static_cast<const MouseMoveEvent&>(e);
		this->HandleMouseMoveEvent(event);
	}
}

void CameraMovementInputHandler::HandleMouseClickEvent(const MouseClickEvent& event)
{
	if (event.button == MouseClickEvent::Button::LEFT)
	{
		if (event.action == KeyOrButtonEvent::Action::PRESS)
			rotationOn = true;
		else if (event.action == KeyOrButtonEvent::Action::RELEASE)
		{
			rotationOn = false;
			firstMouseEventFired = false;
		}
	}
}

void CameraMovementInputHandler::HandleMouseMoveEvent(const MouseMoveEvent& event)
{
	if (rotationOn)
	{
		const glm::dvec2 currentMousePos = glm::dvec2(event.xpos, event.ypos);
		if (!firstMouseEventFired)
		{
			this->lastMousePos = currentMousePos;
			firstMouseEventFired = true;
		}
		const glm::dvec2 mouseOffset = currentMousePos - this->lastMousePos;
		this->lastMousePos = currentMousePos;

		this->camera.RotateX((float)(mouseOffset.y / 100.0));
		this->camera.RotateY((float)(mouseOffset.x / 100.0));
	}
}
