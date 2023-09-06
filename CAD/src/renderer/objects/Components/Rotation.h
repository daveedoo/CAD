#pragma once

struct Rotation
{
	float roll;		// around X
	float pitch;	// around Z
	float yaw;		// around Y

	Rotation() = default;
	Rotation(float roll, float pitch, float yaw)
		: roll(roll), pitch(pitch), yaw(yaw) {}
};
