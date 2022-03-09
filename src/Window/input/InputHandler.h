#pragma once
#include "InputEvent.h"

class InputHandler
{
public:
	virtual void ProcessInput(const InputEvent& event) = 0;
};
