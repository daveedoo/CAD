#pragma once
#include "ICurveSegmentsMetrics.h"
#include <memory>
#include "../../../Camera.h"
#include "../../../ScreenSizeChangePublisher.h"

class BernsteinPolygonMetrics : public ICurveSegmentsMetrics, public ScreenSizeChangeSubscriber
{
private:
	std::shared_ptr<Camera> camera;
	int scrWidth;
	int scrHeight;

public:
	BernsteinPolygonMetrics(std::shared_ptr<Camera> camera, int scrWidth, int scrHeight);

	virtual unsigned int CalculateSegmentsCount(std::array<glm::vec3, 4> curveCoefficients) override;

	// Inherited via ScreenSizeChangeSubscriber
	void OnScreenSizeChanged(unsigned int width, unsigned int height) override;
};
