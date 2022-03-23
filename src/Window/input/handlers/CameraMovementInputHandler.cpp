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
	else if (e.type == InputEvent::EventType::MOUSE_SCROLL)
	{
		const MouseScrollEvent& event = static_cast<const MouseScrollEvent&>(e);
		this->HandleScrollEvent(event);
	}
}

void CameraMovementInputHandler::HandleMouseClickEvent(const MouseClickEvent& event)
{
	if (event.button == MouseClickEvent::Button::LEFT)
	{
		if (event.action == KeyOrButtonEvent::Action::PRESS)
			this->rotationOn = true;
		else if (event.action == KeyOrButtonEvent::Action::RELEASE)
		{
			this->rotationOn = false;
			this->lastMousePos = std::nullopt;
		}
	}
	else if (event.button == MouseClickEvent::Button::RIGHT)
	{
		if (event.action == KeyOrButtonEvent::Action::PRESS)
			this->translationOn = true;
		else if (event.action == KeyOrButtonEvent::Action::RELEASE)
		{
			this->translationOn = false;
			this->lastMousePos = std::nullopt;
		}
	}
}

void CameraMovementInputHandler::HandleMouseMoveEvent(const MouseMoveEvent& event)
{
	if (rotationOn || translationOn)
	{
		const glm::dvec2 currentMousePos = glm::dvec2(event.xpos, -event.ypos);	// negative Y, because GLFW window origin is left-top
		if (!this->lastMousePos.has_value())
			this->lastMousePos = currentMousePos;
		const glm::dvec2 mouseOffset = currentMousePos - this->lastMousePos.value();
		this->lastMousePos = currentMousePos;
		
		if (rotationOn)
		{
			this->camera.RotateX((float)(-mouseOffset.y / 100.0));	// negative, because yes
			this->camera.RotateY((float)(mouseOffset.x / 100.0));
		}
		if (translationOn)
		{
			this->camera.Translate(glm::vec3(mouseOffset.x / 100.0, mouseOffset.y / 100.0, 0));
		}
	}
}

void CameraMovementInputHandler::HandleScrollEvent(const MouseScrollEvent& event)
{
	static constexpr double sensitivity = 5.0 / 100.0;	// TODO: take it out

	if (event.yoffset > 0)
		this->camera.Scale((float)(1.0 + sensitivity * event.yoffset));
	else
		this->camera.Scale((float)(1.0/(1.0 - sensitivity * event.yoffset)));
}
