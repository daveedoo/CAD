#include "CameraSubscriber.h"
#include "Window/input/handlers/CameraMovementInputHandler.h"

void CameraSubscriber::AddSubject(std::shared_ptr<CameraMovementInputHandler> cameraHandler)
{
	cameraHandler->AddSubscriber(shared_from_this());
}
