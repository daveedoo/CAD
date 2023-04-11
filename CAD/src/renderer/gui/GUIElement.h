#pragma once
#include <imgui.h>
#include <imgui_stdlib.h>

class GUIElement
{
public:
	virtual void Draw() = 0;
};

