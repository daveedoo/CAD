#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <tuple>
#include "../renderer/objects/Components/Rotation.h"

static class RotationRepresentationsConverter
{
public:
	static Rotation ConvertToAxisAngle(const glm::quat& q);
	// All three angles are in degrees
	static Rotation ConvertToAxisAngle(const glm::vec3 basisAxesRotation);

private:
	// TODO: naming to be the other way
	// First tuple component is lambda - angle from +X to +Z
	// Second tuple component is fi - angle from XZ plane to +Y
	static std::tuple<float, float> ConvertToSphericalCoordinates(const glm::vec3& direction);
};

