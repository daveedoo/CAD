#include "GroupTransformationWindow.h"
#include "..\objects\Components\ScaleRotation.h"
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
	cancelCommand(std::move(cancelCommand))
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
		bool transfChanged = false;
		if (ImGui::DragFloat("Scale", &additionalTransformation->scale, 0.01f)) transfChanged = true;
		if (ImGui::DragFloat("Axis Lambda", &additionalTransformation->rotation.axisLambda, 0.1f)) transfChanged = true;
		if (ImGui::DragFloat("Axis Fi", &additionalTransformation->rotation.axisFi, 0.1f)) transfChanged = true;
		if (ImGui::DragFloat("Angle", &additionalTransformation->rotation.angle, 0.1f)) transfChanged = true;
		
		static glm::vec3 axisRotation = glm::vec3(0.f);
		bool axisRotationChanged = false;
		if (ImGui::DragFloat("X", &axisRotation.x, 0.1f)) axisRotationChanged = true;
		if (ImGui::DragFloat("Y", &axisRotation.y, 0.1f)) axisRotationChanged = true;
		if (ImGui::DragFloat("Z", &axisRotation.z, 0.1f)) axisRotationChanged = true;
		if (axisRotationChanged)
		{
			auto temp = RotationRepresentationsConverter::ConvertToAxisAngle(axisRotation);
			additionalTransformation->rotation = Rotation(temp.axisFi, temp.axisLambda, temp.angle);
			changeCommand->execute();
		}
		
		if (transfChanged)
			changeCommand->execute();

		if (ImGui::Button("Apply"))
		{
			groupTransformationStarted = false;

			applyCommand->execute();
		}
		if (ImGui::Button("Cancel"))
		{
			groupTransformationStarted = false;
			cancelCommand->execute();
		}
	}
}
