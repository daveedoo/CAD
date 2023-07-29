#include "RotationRepresentationsConverter.h"
#include "Matrix.h"
#include <numbers>

AxisAngleRotation RotationRepresentationsConverter::ConvertToAxisAngle(const glm::quat& q)
{
	float th = 2.f * glm::acos(q.w);
	glm::vec3 axis = glm::vec3(q.x, q.y, q.z) / glm::sin(th / 2.f);
	auto [lambda, fi] = ConvertToSphericalCoordinates(axis);

	return AxisAngleRotation(glm::degrees(fi), glm::degrees(lambda), glm::degrees(th));
}

AxisAngleRotation RotationRepresentationsConverter::ConvertToAxisAngle(const glm::vec3 axesRotation)
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

	return AxisAngleRotation(glm::degrees(fi), glm::degrees(lambda), glm::degrees(theta));
}

EulerAngles RotationRepresentationsConverter::ConvertToRPY(const glm::quat& q)
{
	// this implementation assumes normalized quaternion
	// converts to Euler angles in 3-2-1 sequence
	EulerAngles angles;

	// roll
	double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.roll = std::atan2(sinr_cosp, cosr_cosp);

	// pitch
	double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.pitch = std::atan2(siny_cosp, cosy_cosp);

	// yaw
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angles.yaw = 2 * std::atan2(sinp, cosp) - std::numbers::pi / 2;

	return angles;
}

std::tuple<float, float> RotationRepresentationsConverter::ConvertToSphericalCoordinates(const glm::vec3& v)
{
	float lambda = atan2(v.z, v.x);
	float fi = atan2(v.y, sqrt(v.x * v.x + v.z * v.z));

	return std::make_tuple(lambda, fi);
}
