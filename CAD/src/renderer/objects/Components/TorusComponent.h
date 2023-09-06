#pragma once

struct TorusComponent
{
	float minorR;
	float majorR;
	int minorSegments;
	int majorSegments;

	TorusComponent(float minorR, float majorR, int minorSegments, int majorSegments)
		: minorR(minorR), majorR(majorR), minorSegments(minorSegments), majorSegments(majorSegments) {}
};

