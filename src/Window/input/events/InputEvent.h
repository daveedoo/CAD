#pragma once

class InputEvent
{
public:
	enum class EventType
	{
		KEY,
		MOUSE_CLICK,
		MOUSE_MOVE,
		MOUSE_SCROLL,
		RESIZE
	};

public:
	EventType type;

protected:
	InputEvent(EventType type) :
		type(type) {}
};
