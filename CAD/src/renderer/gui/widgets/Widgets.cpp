#include "Widgets.h"
#include <imgui.h>

namespace ImGui
{
	bool RotationRPY(Rotation& rotation)
	{
		bool change = false;
		if (ImGui::DragFloat("X", &rotation.roll, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) change = true;
		if (ImGui::DragFloat("Y", &rotation.yaw, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) change = true;
		if (ImGui::DragFloat("Z", &rotation.pitch, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) change = true;
		return change;
	}
	bool RotationAxisAngle(AxisAngleRotation& rotation)
	{
		bool change = false;
		if (ImGui::DragFloat("Axis Lambda", &rotation.axisLambda, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) change = true;
		if (ImGui::DragFloat("Axis Fi", &rotation.axisFi, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) change = true;
		if (ImGui::DragFloat("Angle", &rotation.angle, 0.1f, -360.f, 360.f, "%.3f", ImGuiSliderFlags_AlwaysClamp)) change = true;
		return change;
	}
}

