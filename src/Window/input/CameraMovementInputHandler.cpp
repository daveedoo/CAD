#include "CameraMovementInputHandler.h"
#include <GLFW/glfw3.h>

void CameraMovementInputHandler::ProcessInput(const InputEvent& e)
{
	if (e.type != InputEvent::EventType::KEY)
		return;

	const KeyEvent& event = static_cast<const KeyEvent&>(e);
	if (event.action == GLFW_PRESS || event.action == GLFW_REPEAT)
	{
		if (event.key == GLFW_KEY_D)
		{
			this->camera.RotateX(0.1);
		}
	}
}
