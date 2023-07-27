#pragma once

struct Rotation
{
	float axisFi;
	float axisLambda;
	float angle;

	Rotation(float axisFi = 0.f, float axisLambda = 0.f, float angle = 0.f)
		: axisFi(axisFi), axisLambda(axisLambda), angle(angle) {}
};
