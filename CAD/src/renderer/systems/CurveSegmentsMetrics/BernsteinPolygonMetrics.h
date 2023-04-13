#pragma once
#include "ICurveSegmentsMetrics.h"
#include <memory>
#include "../../../Camera.h"

class BernsteinPolygonMetrics : public ICurveSegmentsMetrics
{
private:
	std::shared_ptr<Camera> camera;
	int scrWidth;
	int scrHeight;

public:
	BernsteinPolygonMetrics(std::shared_ptr<Camera> camera, int scrWidth, int scrHeight);
	void UpdateScreenSize(int scrWidth, int scrHeight);

	virtual unsigned int CalculateSegmentsCount(std::array<glm::vec3, 4> curveCoefficients) override;
};
