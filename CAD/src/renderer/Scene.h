#pragma once
#include <memory>
#include "../Camera.h"
#include "../Window/input/events/InputEvent.h"
#include "../Window/input/handlers/InputHandler.h"
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VAO.h"
#include "objects/Ellipsoid/Ellipsoid.h"
#include "objects/Ellipsoid/Coefficients.h"

class Scene
{
private:
	std::unique_ptr<Camera> camera;

	std::unique_ptr<InputHandler> cameraMovementHandler;

public:
	std::unique_ptr<Ellipsoid> ellipsoid;
	Scene();

	void Update();
	void Render();

	void HandleEvent(const InputEvent& inputEvent);
	void SetFramebufferSize(unsigned int width, unsigned int height);
};
