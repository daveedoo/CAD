#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <tuple>
#include "EulerAngles.h"
#include "AxisAngleRotation.h"

static class RotationRepresentationsConverter
{
public:
	static AxisAngleRotation ConvertToAxisAngle(const glm::quat& q);
	// All three angles are in degrees
	static AxisAngleRotation ConvertToAxisAngle(const glm::vec3 basisAxesRotation);

	static EulerAngles ConvertToRPY(const glm::quat& q);

private:
	// TODO: naming to be the other way
	// First tuple component is lambda - angle from +X to +Z
	// Second tuple component is fi - angle from XZ plane to +Y
	static std::tuple<float, float> ConvertToSphericalCoordinates(const glm::vec3& direction);
};
