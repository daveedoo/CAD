#pragma once
#include "../Window/Window.h"
#include "../Camera.h"
#include "Scene.h"
//#include "GUI.h"

/// <summary>
/// Class responsible for rendering everything (scene and the GUI) and handling the input.
/// </summary>
class Renderer
{
private:
	/// This class contains the Window object as it needs it to check the framebuffer size on resize event.
	Window& window;
	int framebufferWidth;
	int framebufferHeight;

	std::unique_ptr<Scene> scene;
	//std::unique_ptr<GUI> gui;

	// adapive shading stuff
	bool adaptiveShadingOn = true;
	bool sceneIsMoving = false;
	int adaptiveLvl = 8;
	std::unique_ptr<GL::Program> adaptiveShadingProgram;
	std::unique_ptr<GL::VAO> adaptiveShadingVAO;
	std::unique_ptr<GL::VBO> adaptiveShadingVBO;
	std::unique_ptr<GL::FBO> adaptiveShadingFBO;
	std::shared_ptr<GL::Texture2D> adaptiveShadingTexture;

	void UpdateMaxAdaptiveLvl();
	/// This method sends info to the `scene` object that frame size has changed
	/// and updates the adaptive texture accordingly.
	void UpdateSceneFramebufferSize();
	bool ShouldBeRenderedAdaptively() const;

public:
	Renderer(Window& window);
	~Renderer();

	void Update();
	void Render();

private:
	void RenderGUI();
};
