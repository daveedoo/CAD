#pragma once
#include <vector>
#include <entt/entt.hpp>

struct BezierC2
{
	std::vector<entt::entity> deBoorPoints;
	bool polylineVisible;

	BezierC2(const std::vector<entt::entity>& deBoorPoints, bool polylineVisible = true)
		: deBoorPoints(deBoorPoints), polylineVisible(polylineVisible) {}
};
