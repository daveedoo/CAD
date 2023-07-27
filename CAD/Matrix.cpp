#include "Matrix.h"
#include <glm\gtx\matrix_decompose.hpp>
#include "src\renderer\objects\Components\Rotation.h"

glm::mat4 Matrix::PerspectiveProjection(float fov, float aspect, float n, float f)
{
	float ctg = 1 / glm::tan(glm::radians(fov / 2.f));
	return glm::mat4(
		ctg / aspect, 0, 0, 0,
		0, ctg, 0, 0,
		0, 0, -(f + n) / (f - n), -1.f,
		0, 0, (-2 * f * n) / (f - n), 0
	);
}

glm::mat4 Matrix::Scale(float ratio)
{
	return glm::mat4(
		ratio, 0.f, 0.f, 0.f,
		0.f, ratio, 0.f, 0.f,
		0.f, 0.f, ratio, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

glm::mat4 Matrix::Scale(glm::vec3 scale)
{
	return glm::mat4(
		scale.x, 0.f, 0.f, 0.f,
		0.f, scale.y, 0.f, 0.f,
		0.f, 0.f, scale.z, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

glm::mat4 Matrix::LookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	const glm::vec3 newZ = glm::normalize(eye - center);
	const glm::vec3 newX = glm::normalize(glm::cross(up, newZ));	//
	const glm::vec3 newY = glm::cross(newZ, newX);

	return glm::mat4(
		newX.x, newY.x, newZ.x, 0.f,
		newX.y, newY.y, newZ.y, 0.f,
		newX.z, newY.z, newZ.z, 0.f,
		-glm::dot(newX, eye), -glm::dot(newY, eye), -glm::dot(newZ, eye), 1.f
	);
}

glm::mat4 Matrix::RotationX(float angle)
{
	float cos = glm::cos(angle);
	float sin = glm::sin(angle);
	return glm::mat4(
		1, 0, 0, 0,
		0, cos, sin, 0,
		0, -sin, cos, 0,
		0, 0, 0, 1
	);
}

glm::mat4 Matrix::RotationY(float angle)
{
	float cos = glm::cos(angle);
	float sin = glm::sin(angle);
	return glm::mat4(
		cos, 0.f, -sin, 0.f,
		0.f, 1.f, 0.f, 0.f,
		sin, 0.f, cos, 0.f,
		0.f, 0.f, 0.f, 1.f
	);
}

glm::mat4 Matrix::RotationZ(float angle)
{
	float cos = glm::cos(angle);
	float sin = glm::sin(angle);
	return glm::mat4(
		cos, sin, 0, 0,
		-sin, cos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

glm::mat4 Matrix::Translation(glm::vec3 v)
{
	auto mat = glm::mat4(1.f);
	mat[3][0] = v.x;
	mat[3][1] = v.y;
	mat[3][2] = v.z;

	return mat;
}

glm::mat4 Matrix::Rotate(const Rotation& sr)
{
	float lambda = glm::radians(sr.axisLambda);
	float fi = glm::radians(sr.axisFi);
	float a = glm::radians(sr.angle);

	float x = glm::cos(fi) * glm::cos(lambda);
	float z = glm::cos(fi) * glm::sin(lambda);
	float y = glm::sin(fi);
	float s = glm::sin(a);
	float c = glm::cos(a);
	float t = 1 - c;

	return glm::mat4{
		t*x*x + c,		t*x*y + s*z,	t*x*z - s*y,	0,
		t*x*y - s*z,	t*y*y + c,		t*y*z + s*x,	0,
		t*x*z + s*y,	t*y*z - s*x,	t*z*z + c,		0,
		0,				0,				0,				1
	};
}

glm::mat4 Matrix::RotationAroundPoint(const AdditionalTransformation& addTransf, glm::vec3 objectPosition)
{
	glm::vec3 T = addTransf.centerPoint - objectPosition;
	return Matrix::Translation(T) * Matrix::Rotate(addTransf.rotation) * Matrix::Translation(-T);
}

glm::mat4 Matrix::GetResultingTransformationMatrix(const Position& position, const Scaling* scale, const Rotation* scaleRot, const AdditionalTransformation* addTransf)
{
	glm::mat4 worldMtx = glm::mat4(1.f);

	if (addTransf == nullptr)
	{
		// 1. Scale,	2. Rotate,	3. Translate
		glm::vec3 resPosition = position.position;
		worldMtx *= Matrix::Translation(resPosition);

		if (scale != nullptr && scaleRot != nullptr)
		{
			worldMtx *= Matrix::Rotate(*scaleRot) * Matrix::Scale(*scale);
		}
	}
	else
	{
		// 1. Scale (self + addit.),	2. Rotate (self),	3. Rotate (around addit. point),	4. Translate (self + resulting from scaling)
		glm::vec3 resPosition = addTransf->scale * (position.position - addTransf->centerPoint) + addTransf->centerPoint;
		worldMtx *= Matrix::Translation(resPosition);
		worldMtx *= Matrix::RotationAroundPoint(*addTransf, position.position);

		if (scale != nullptr && scaleRot != nullptr)
		{
			glm::vec3 resScale = *scale;
			resScale *= addTransf->scale;
			worldMtx *= Matrix::Rotate(*scaleRot) * Matrix::Scale(resScale);
		}
	}
	return worldMtx;
}

void Matrix::Decompose(const glm::mat4& mat, glm::vec3& scale, glm::quat& rotation, glm::vec3& translation)
{
	glm::vec3 _skew;
	glm::vec4 _persp;
	glm::decompose(mat, scale, rotation, translation, _skew, _persp);
}
