#include "GroupTransformationWindow.h"
#include "..\objects\Components\ScaleRotation.h"
#include <glm\gtc\type_ptr.hpp>
#include "..\..\maths\RotationRepresentationsConverter.h"

GroupTransformationWindow::GroupTransformationWindow(std::shared_ptr<ScaleRotation> scaleRotation,
	std::shared_ptr<Command> startCommand,
	std::shared_ptr<Command> changeCommand,
	std::shared_ptr<Command> applyCommand,
	std::shared_ptr<Command> cancelCommand)
	: GUIElement(), scaleRotation(scaleRotation),
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
		if (ImGui::DragFloat("Scale", &scaleRotation->scale.x, 0.01f))
		{
			scaleRotation->scale.y = scaleRotation->scale.z = scaleRotation->scale.x;
			transfChanged = true;
		}
		if (ImGui::DragFloat("Axis Lambda", &scaleRotation->axisLambda, 0.1f)) transfChanged = true;
		if (ImGui::DragFloat("Axis Fi", &scaleRotation->axisFi, 0.1f)) transfChanged = true;
		if (ImGui::DragFloat("Angle", &scaleRotation->angle, 0.1f)) transfChanged = true;
		
		static glm::vec3 axisRotation = glm::vec3(0.f);
		bool axisRotationChanged = false;
		if (ImGui::DragFloat("X", &axisRotation.x, 0.1f)) axisRotationChanged = true;
		if (ImGui::DragFloat("Y", &axisRotation.y, 0.1f)) axisRotationChanged = true;
		if (ImGui::DragFloat("Z", &axisRotation.z, 0.1f)) axisRotationChanged = true;
		if (axisRotationChanged)
		{
			*scaleRotation = RotationRepresentationsConverter::ConvertToAxisAngle(axisRotation);
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
