#pragma once
#include "../Camera.h"
#include <memory>
#include "../Window/input/events/InputEvent.h"
#include "../Window/input/handlers/InputHandler.h"
#include "Coefficients.h"
#include "../gl/wrappers/Program.h"
#include "../gl/wrappers/VAO.h"
#include "Elipsoide.h"

class Scene
{
private:
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Elipsoide> elipsoide;

	std::unique_ptr<InputHandler> cameraMovementHandler;

public:
	Scene();

	void HandleEvent(const InputEvent& inputEvent);
	void Draw(Coefficients& coefficients);
};
