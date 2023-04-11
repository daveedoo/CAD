#pragma once
#include "System.h"
#include "../../gl/wrappers/Program.h"

class TorusSystem : public System
{
private:
	std::unique_ptr<GL::Program> torusProgram;

public:
	TorusSystem(std::shared_ptr<entt::registry> registry);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

private:
	glm::vec3 GetObjectColor(bool isSelected);
};
