#pragma once
#include <glm\glm.hpp>
#include "../../objects/Components/Rotation.h"
#include "../../../maths/AxisAngleRotation.h"

namespace ImGui
{
	bool RotationRPY(Rotation& rotation);
	bool RotationAxisAngle(AxisAngleRotation& rotation);
}