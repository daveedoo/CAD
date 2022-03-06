#pragma once
#include "Window.h"

class Renderer
{
private:
	const Window& window;

public:
	Renderer(const Window& window);
	void DrawScene();
};
