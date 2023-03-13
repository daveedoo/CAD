#pragma once
#include "InputEvent.h"

class MouseMoveEvent : public InputEvent
{
public:
	/// <summary>
	/// Coordinates of the mouse position relative to the origin in top-left corner.
	/// </summary>
	double xpos, ypos;
	MouseMoveEvent(double xpos, double ypos) : InputEvent(EventType::MOUSE_MOVE),
		xpos(xpos), ypos(ypos) {}
};
