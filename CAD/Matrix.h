#pragma once
#include <glm/glm.hpp>
#include "src\renderer\objects\Components\AdditionalTransformation.h"
#include "src\renderer\objects\Components\ScaleRotation.h"
#include "src\renderer\objects\Components\Rotation.h"
#include "src\renderer\objects\Components\Scaling.h"
#include "src\renderer\objects\Components\Position.h"

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

	static glm::mat4 Rotate(const Rotation& rotation);
	static glm::mat4 RotationAroundPoint(const AdditionalTransformation& addTransf, glm::vec3 objectPosition);

	static glm::mat4 GetResultingTransformationMatrix(const Position& position, const Scaling* scale, const Rotation* scaleRot, const AdditionalTransformation* addTransf);

	static void Decompose(const glm::mat4& mat, glm::vec3& scale, glm::quat& rotation, glm::vec3& translation);
};
