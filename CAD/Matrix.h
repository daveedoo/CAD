#pragma once
#include <glm/glm.hpp>
#include "src\renderer\objects\Components\AdditionalTransformation.h"

static class Matrix
{
public:
	static glm::mat4 PerspectiveProjection(float fov, float aspect, float near, float far);
	static glm::mat4 Scale(float ratio);
	static glm::mat4 Scale(glm::vec3 scale);
	static glm::mat4 LookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

	static glm::mat4 RotationX(float angle);
	static glm::mat4 RotationY(float angle);
	static glm::mat4 RotationZ(float angle);
	static glm::mat4 Translation(glm::vec3 v);

	static glm::mat4 Rotation(const ScaleRotation& scaleRotation);
	static glm::mat4 RotationAroundPoint(const AdditionalTransformation& addTransf, glm::vec3 objectPosition);
};
