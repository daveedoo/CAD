#pragma once

class ScreenSizeChangeSubscriber
{
public:
	virtual void OnScreenSizeChanged(unsigned int width, unsigned int height) = 0;
};
