#pragma once
#include <vector>
#include <memory>
#include "ScreenSizeChangeSubscriber.h"

template<typename TSubscriber>
class Publisher
{
private:
	std::vector<std::weak_ptr<TSubscriber>> subscribers;

public:
	void NotifySubscribers();
};

class ScreenSizeChangePublisher
{
private:
	std::vector<std::shared_ptr<ScreenSizeChangeSubscriber>> subscribers;

public:
	void AddSubscriber(std::shared_ptr<ScreenSizeChangeSubscriber> subscriber);
	void NotifySubscribers();
};
