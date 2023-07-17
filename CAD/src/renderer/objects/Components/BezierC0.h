#pragma once
#include <vector>
#include <entt/entt.hpp>

struct BezierC0
{
	std::vector<entt::entity> points;

	BezierC0(const std::vector<entt::entity>& points)
		: points(points) {}
};

