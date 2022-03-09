#pragma once
struct InputEvent
{
	enum class EventType
	{
		KEY,
		MOUSE
	};

	EventType type;

protected:
	InputEvent(EventType type) :
		type(type) {}
};

struct KeyEvent : InputEvent
{
	int key, scancode, action, mods;
	KeyEvent(int key, int scancode, int action, int mods) : InputEvent(EventType::KEY),
		key(key), scancode(scancode), action(action), mods(mods) {}
};

struct MouseEvent : InputEvent
{
	int button, action, mods;
	MouseEvent(int button, int action, int mods) : InputEvent(EventType::MOUSE),
		button(button), action(action), mods(mods) {}
};

//struct MouseMoveEvent : InputEvent
//{
//	double xpos, ypos;
//	MouseMoveEvent(double xpos, double ypos) :
//		xpos(xpos), ypos(ypos) {}
//};
