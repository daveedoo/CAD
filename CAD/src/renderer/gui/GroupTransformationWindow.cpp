#include "GroupTransformationWindow.h"
#include "..\objects\Components\ScaleRotation.h"
#include <glm\gtc\type_ptr.hpp>

GroupTransformationWindow::GroupTransformationWindow(std::shared_ptr<ScaleRotation> scaleRotation,
	std::shared_ptr<Command> startCommand,
	std::shared_ptr<Command> changeCommand,
	std::shared_ptr<Command> applyCommand,
	std::shared_ptr<Command> cancelCommand)
	: GUIElement(), scaleRotation(scaleRotation),
	startCommand(std::move(startCommand)),
	changeCommand(std::move(changeCommand)),
	applyCommand(applyCommand),
	cancelCommand(cancelCommand)
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
		if (ImGui::DragFloat3("Scale", glm::value_ptr(scaleRotation->scale), 0.01f)) transfChanged = true;
		if (ImGui::DragFloat("Axis Lambda", &scaleRotation->axisLambda, 0.1f)) transfChanged = true;
		if (ImGui::DragFloat("Axis Fi", &scaleRotation->axisFi, 0.1f)) transfChanged = true;
		if (ImGui::DragFloat("Angle", &scaleRotation->angle, 0.1f)) transfChanged = true;
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

