#include "GroupTransformationWindow.h"
#include <glm\gtc\type_ptr.hpp>
#include "..\..\maths\RotationRepresentationsConverter.h"

GroupTransformationWindow::GroupTransformationWindow(std::shared_ptr<AdditionalTransformation> additionalTransformation,
	std::shared_ptr<Command> startCommand,
	std::shared_ptr<Command> changeCommand,
	std::shared_ptr<Command> applyCommand,
	std::shared_ptr<Command> cancelCommand)
	: GUIElement(), additionalTransformation(additionalTransformation),
	startCommand(std::move(startCommand)),
	changeCommand(std::move(changeCommand)),
	applyCommand(std::move(applyCommand)),
	cancelCommand(std::move(cancelCommand)),
	xyzRotation(glm::vec3(0.f))
{
}

void GroupTransformationWindow::Draw()
{
	if (!groupTransformationStarted)
	{
		if (ImGui::Button("Transform group"))
		{
			groupTransformationStarted = true;
			startCommand->execute();
		}
	}
	else
	{
		static std::string representations[] = { "X-Y-Z basis axes", "Axis-angle" };
		static bool idx = 0;
		ImGui::SetNextItemWidth(-FLT_MIN);
		ImGui::BeginDisabled(true);
		if (ImGui::BeginCombo("##rotation representation", representations[idx].c_str()))
		{
			for (size_t i = 0; i < 2; i++)
			{
				if (ImGui::Selectable(representations[i].c_str(), idx == i))
					idx = i;
			}
			ImGui::EndCombo();
		}
		ImGui::EndDisabled();
		
		static bool transfChanged = false;
		if (ImGui::DragFloat("Scale", &additionalTransformation->scale, 0.01f)) transfChanged = true;
		if (representations[idx] == "X-Y-Z basis axes")
		{
			bool axisRotationChanged = false;
			if (ImGui::DragFloat("X", &this->xyzRotation.x, 0.1f)) axisRotationChanged = true;
			if (ImGui::DragFloat("Y", &this->xyzRotation.y, 0.1f)) axisRotationChanged = true;
			if (ImGui::DragFloat("Z", &this->xyzRotation.z, 0.1f)) axisRotationChanged = true;

			if (axisRotationChanged)
			{
				auto temp = RotationRepresentationsConverter::ConvertToAxisAngle(this->xyzRotation);
				additionalTransformation->rotation = Rotation(temp.axisFi, temp.axisLambda, temp.angle);
				changeCommand->execute();
			}
		}
		else
		{
			if (ImGui::DragFloat("Axis Lambda", &additionalTransformation->rotation.axisLambda, 0.1f)) transfChanged = true;
			if (ImGui::DragFloat("Axis Fi", &additionalTransformation->rotation.axisFi, 0.1f)) transfChanged = true;
			if (ImGui::DragFloat("Angle", &additionalTransformation->rotation.angle, 0.1f)) transfChanged = true;
		}

		if (transfChanged)
		{
			changeCommand->execute();
			transfChanged = false;
		}

		if (ImGui::Button("Apply"))
		{
			groupTransformationStarted = false;
			this->xyzRotation = glm::vec3(0.f);
			applyCommand->execute();
		}
		if (ImGui::Button("Cancel"))
		{
			groupTransformationStarted = false;
			this->xyzRotation = glm::vec3(0.f);
			cancelCommand->execute();
		}
	}
}
