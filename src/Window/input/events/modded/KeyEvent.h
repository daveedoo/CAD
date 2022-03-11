#pragma once
#include "KeyOrButtonEvent.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <boost/format.hpp>

class KeyEvent : public KeyOrButtonEvent
{
public:
	Action action;
	int key, scancode;

	KeyEvent(int key, int scancode, int action, int mods) : KeyOrButtonEvent(EventType::KEY, mods),
		key(key), scancode(scancode)
	{
		switch (action)
		{
		case GLFW_PRESS:
			this->action = Action::PRESS;
			break;
		case GLFW_RELEASE:
			this->action = Action::RELEASE;
			break;
		case GLFW_REPEAT:
			this->action = Action::REPEAT;
			break;
		default:
			throw std::invalid_argument((boost::format("Invalid argument 'action', value: %1%\n") % action).str());
		}
	}
};
