#pragma once
#include "InputEvent.h"

class MouseMoveEvent : public InputEvent
{
public:
	double xpos, ypos;
	MouseMoveEvent(double xpos, double ypos) : InputEvent(EventType::MOUSE_MOVE),
		xpos(xpos), ypos(ypos) {}
};
