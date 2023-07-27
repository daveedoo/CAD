#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <tuple>
#include "../renderer/objects/Components/ScaleRotation.h"

static class RotationRepresentationsConverter
{
public:
	// TODO: return Rotation component
	static ScaleRotation ConvertToAxisAngle(const glm::quat& q);
	// TODO: return Rotation component
	// All three angles to be in degrees
	static ScaleRotation ConvertToAxisAngle(const glm::vec3 basisAxesRotation);

private:
	// TODO: naming to be the other way
	// First tuple component is lambda - angle from +X to +Z
	// Second tuple component is fi - angle from XZ plane to +Y
	static std::tuple<float, float> ConvertToSphericalCoordinates(const glm::vec3& direction);
};

