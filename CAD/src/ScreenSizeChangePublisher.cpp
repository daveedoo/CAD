#include "ScreenSizeChangePublisher.h"

void ScreenSizeChangePublisher::AddSubscriber(std::shared_ptr<ScreenSizeChangeSubscriber> subscriber)
{
	subscribers.push_back(subscriber);
}

void ScreenSizeChangePublisher::NotifySubscribers()
{
	for (auto& subscriber : this->subscribers)
	{
		subscriber->OnScreenSizeChanged();
	}
}
