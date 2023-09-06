#pragma once
#include <vector>
#include <memory>
#include "ScreenSizeChangeSubscriber.h"

class ScreenSizeChangePublisher
{
private:
	std::vector<std::shared_ptr<ScreenSizeChangeSubscriber>> subscribers;

public:
	void AddScreenSizeSubscriber(std::shared_ptr<ScreenSizeChangeSubscriber> subscriber);
	void NotifySubscribers(unsigned int width, unsigned int height);
};
