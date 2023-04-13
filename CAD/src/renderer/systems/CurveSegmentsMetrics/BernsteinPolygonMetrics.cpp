#include "BernsteinPolygonMetrics.h"

BernsteinPolygonMetrics::BernsteinPolygonMetrics(std::shared_ptr<Camera> camera, int scrWidth, int scrHeight) : ICurveSegmentsMetrics(),
	camera(camera), scrWidth(scrWidth), scrHeight(scrHeight)
{
}

void BernsteinPolygonMetrics::UpdateScreenSize(int scrWidth, int scrHeight)
{
	this->scrWidth = scrWidth;
	this->scrHeight = scrHeight;
}

unsigned int BernsteinPolygonMetrics::CalculateSegmentsCount(std::array<glm::vec3, 4> curveCoefficients)
{
	auto viewProj = this->camera->GetProjectionMatrix() * this->camera->GetViewMatrix();

	std::array<glm::vec2, 4> bezierPoints;
	for (size_t i = 0; i < 4; i++)
	{
		auto bi = glm::vec4(curveCoefficients[i], 1.f);
		auto bi_scr = viewProj * bi;
		bi_scr /= bi_scr.w;

		bezierPoints[i] = bi_scr;
	}

	double length = 0.0;
	for (size_t i = 0; i < 4; i++)
	{
		const auto& p1 = bezierPoints[(i + 0) % 4];
		const auto& p2 = bezierPoints[(i + 1) % 4];

		float w = ((p1.x - p2.x) / 2) * scrWidth;
		float h = ((p1.y - p2.y) / 2) * scrHeight;
		length += sqrt(w * w + h * h);
	}
	return glm::max(256.0, length);
}
