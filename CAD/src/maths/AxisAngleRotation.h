#pragma once
struct AxisAngleRotation
{
	// TODO: naming to be the other way
	
	// Angle from XZ plane to +Y
	float axisFi;

	// TODO: this angle is in negative direction
	// Angle from +X to +Z
	float axisLambda;
	
	float angle;

	AxisAngleRotation(float axisFi = 0.f, float axisLambda = 0.f, float angle = 0.f)
		: axisFi(axisFi), axisLambda(axisLambda), angle(angle) {}
};