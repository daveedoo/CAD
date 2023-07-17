#pragma once
#include "GUIElement.h"
#include "../commands/Command.h"
#include <memory>
#include "../../renderer/objects/Components/ScaleRotation.h"

class GroupTransformationWindow : public GUIElement
{
private:
	bool groupTransformationStarted = false;
	std::shared_ptr<ScaleRotation> scaleRotation;
	std::shared_ptr<Command> startCommand;
	std::shared_ptr<Command> changeCommand;
	std::shared_ptr<Command> applyCommand;
	std::shared_ptr<Command> cancelCommand;

public:
	GroupTransformationWindow(std::shared_ptr<ScaleRotation> scaleRotation,
		std::shared_ptr<Command> startCommand,
		std::shared_ptr<Command> changeCommand,
		std::shared_ptr<Command> applyCommand,
		std::shared_ptr<Command> cancelCommand);

	virtual void Draw() override;
};
