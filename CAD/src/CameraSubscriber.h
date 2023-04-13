#pragma once
#include <memory>

class CameraMovementInputHandler;

class CameraSubscriber : public std::enable_shared_from_this<CameraSubscriber>
{
public:
	void AddSubject(std::shared_ptr<CameraMovementInputHandler> cameraHandler);
	virtual void NotifyCameraMove() = 0;
};
