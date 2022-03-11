#pragma once
#include "Window/Window.h"
#include "Camera.h"

class Renderer
{
private:
	const Window& window;

public:
	Renderer(const Window& window);

	void DrawScene(const Camera& camera);
};
