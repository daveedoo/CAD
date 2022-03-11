#pragma once
#include "KeyOrButtonEvent.h"
#include <stdexcept>
#include <boost/format.hpp>

class MouseClickEvent : public KeyOrButtonEvent
{
public:
	enum class Button
	{
		LEFT = GLFW_MOUSE_BUTTON_LEFT,
		RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
		MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
	};
	Button button;
	Action action;

public:
	MouseClickEvent(int button, int action, int mods) : KeyOrButtonEvent(EventType::MOUSE_CLICK, mods)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			this->button = Button::LEFT;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			this->button = Button::RIGHT;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			this->button = Button::MIDDLE;
			break;
		default:
			throw std::invalid_argument((boost::format("Invalid argument 'button', value: %1%.\n") % button).str());
		}

		switch (action)
		{
		case GLFW_PRESS:
			this->action = Action::PRESS;
			break;
		case GLFW_RELEASE:
			this->action = Action::RELEASE;
			break;
		default:
			throw std::invalid_argument((boost::format("Invalid argument 'action', value: %1%\n") % action).str());
		}
	}
};
