#pragma once
#include "InputEvent.h"
#include "InputHandler.h"
#include "../Camera.h"

class CameraMovementInputHandler : public InputHandler
{
private:
	Camera& camera;

public:
	CameraMovementInputHandler(Camera& camera) :
		camera(camera) {};

	void ProcessInput(const InputEvent& event) override;
};
