#pragma once
#include "Command.h"
#include <entt\entt.hpp>
#include <memory>
#include "..\EntitiesFactory.h"

class AddPointCommand : public Command
{
private:
	std::shared_ptr<entt::registry> registry;
	std::shared_ptr<EntitiesFactory> entitiesFactory;
	entt::entity mainCursor;
	//entt::entity lastCreatedPoint;

public:
	AddPointCommand(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory, entt::entity mainCursor);
	//entt::entity GetLastCreatedPoint() const { return this->lastCreatedPoint; }

	// Inherited via Command
	virtual void execute() override;
};
