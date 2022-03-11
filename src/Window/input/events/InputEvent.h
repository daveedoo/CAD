#pragma once

class InputEvent
{
public:
	enum class EventType
	{
		KEY,
		MOUSE_CLICK,
		MOUSE_MOVE
	};

public:
	EventType type;

protected:
	InputEvent(EventType type) :
		type(type) {}
};
