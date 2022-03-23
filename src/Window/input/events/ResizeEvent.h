#pragma once
#include "InputEvent.h"

class ResizeEvent : public InputEvent
{
public:
	int width, height;
	ResizeEvent(int width, int height) : InputEvent(InputEvent::EventType::RESIZE),
		width(width), height(height) {}
};
