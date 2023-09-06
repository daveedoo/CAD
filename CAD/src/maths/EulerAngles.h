#pragma once
struct EulerAngles
{
	float roll;		// x-axis rotation
	float pitch;	// z-axis rotation
	float yaw;		// y-axis rotation

	EulerAngles() = default;
	EulerAngles(float roll, float pitch, float yaw)
		: roll(roll), pitch(pitch), yaw(yaw) {}
};
