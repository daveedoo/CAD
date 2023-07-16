#pragma once
#include <memory>

class CameraSubscriber : public std::enable_shared_from_this<CameraSubscriber>
{
public:
	virtual void OnCameraMove() = 0;
};
