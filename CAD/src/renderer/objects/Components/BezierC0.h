#pragma once
#include <vector>
#include <entt/entt.hpp>

struct BezierC0
{
	std::vector<entt::entity> points;
	bool polylineVisible;

	BezierC0(const std::vector<entt::entity>& points, bool polylineVisible = true)
		: points(points), polylineVisible(polylineVisible) {}
};
