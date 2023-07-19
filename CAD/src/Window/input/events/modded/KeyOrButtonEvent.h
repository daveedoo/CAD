#pragma once
#include <GLFW/glfw3.h>
#include "../InputEvent.h"

class KeyOrButtonEvent : public InputEvent
{
public:
	enum class Action
	{
		PRESS = GLFW_PRESS,
		RELEASE = GLFW_RELEASE,
		REPEAT = GLFW_REPEAT
	};

	class Mods
	{
	private:
		int mods;

	public:
		Mods(int mods) : mods(mods) {}
		bool IsShiftDown() const { return (this->mods & GLFW_MOD_SHIFT) > 0; }
		bool IsCtrlDown() const { return (this->mods & GLFW_MOD_CONTROL) > 0; }
		bool IsAltDown() const { return (this->mods & GLFW_MOD_ALT) > 0; }
		bool IsCapsLockON() const { return (this->mods & GLFW_MOD_CAPS_LOCK) > 0; }
		bool IsNumLockON() const { return (this->mods & GLFW_MOD_NUM_LOCK) > 0; }
	};

public:
	Mods mods;

protected:
	KeyOrButtonEvent(EventType type, int mods) : InputEvent(type),
		mods(mods) {}
};
