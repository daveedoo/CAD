#pragma once
#include "../Window/Window.h"
#include "../Camera.h"
#include "Scene.h"
#include "GUI.h"

class Renderer
{
private:
	Window& window;

	std::unique_ptr<Scene> scene;
	std::unique_ptr<GUI> gui;

	void UpdateViewport();

public:
	Renderer(Window& window);

	void HandleEvent(const InputEvent& inputEvent);
	void Draw();
};
