#include "RotationRepresentationsConverter.h"
#include "..\..\Matrix.h"

ScaleRotation RotationRepresentationsConverter::ConvertToAxisAngle(const glm::quat& q)
{
	float th = 2.f * glm::acos(q.w);
	glm::vec3 n = glm::vec3(q.x, q.y, q.z) / glm::sin(th / 2.f);
	auto [lambda, fi] = ConvertToSphericalCoordinates(n);

	return ScaleRotation(glm::vec3(1.f), glm::degrees(fi), glm::degrees(lambda), glm::degrees(th));
}

ScaleRotation RotationRepresentationsConverter::ConvertToAxisAngle(const glm::vec3 axesRotation)
{
	// TODO: no need to make a matrix?
	glm::mat4 M =
		Matrix::RotationX(glm::radians(axesRotation.x)) *
		Matrix::RotationY(glm::radians(axesRotation.y)) *
		Matrix::RotationZ(glm::radians(axesRotation.z));

	float theta = glm::acos((M[0][0] + M[1][1] + M[2][2] - 1.f) / 2.f);
	float sin_th = glm::sin(theta);
	float n_x = (M[1][2] - M[2][1]) / (2 * sin_th);
	float n_y = (M[2][0] - M[0][2]) / (2 * sin_th);
	float n_z = (M[0][1] - M[1][0]) / (2 * sin_th);
	glm::vec3 dir = glm::vec3(n_x, n_y, n_z);

	auto [lambda, fi] = ConvertToSphericalCoordinates(dir);

	return ScaleRotation(glm::vec3(1.f), glm::degrees(fi), glm::degrees(lambda), glm::degrees(theta));
}

std::tuple<float, float> RotationRepresentationsConverter::ConvertToSphericalCoordinates(const glm::vec3& v)
{
	float lambda = atan2(v.z, v.x);
	float fi = atan2(v.y, sqrt(v.x * v.x + v.z * v.z));

	return std::make_tuple(lambda, fi);
}
