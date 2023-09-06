#include "ScreenSizeChangePublisher.h"

void ScreenSizeChangePublisher::AddScreenSizeSubscriber(std::shared_ptr<ScreenSizeChangeSubscriber> subscriber)
{
	subscribers.push_back(subscriber);
}

void ScreenSizeChangePublisher::NotifySubscribers(unsigned int width, unsigned int height)
{
	for (auto& subscriber : this->subscribers)
	{
		subscriber->OnScreenSizeChanged(width, height);
	}
}
