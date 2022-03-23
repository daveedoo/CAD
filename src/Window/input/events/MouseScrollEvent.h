#pragma once
#include "InputEvent.h"

class MouseScrollEvent : public InputEvent
{
public:
	double xoffset, yoffset;
	MouseScrollEvent(double xoffset, double yoffset) : InputEvent(InputEvent::EventType::MOUSE_SCROLL),
		xoffset(xoffset), yoffset(yoffset) {}
};
