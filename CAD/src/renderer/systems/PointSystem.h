#pragma once
#include "System.h"
#include "../../gl/wrappers/VAO.h"
#include "../../gl/wrappers/Program.h"

class PointSystem : public System
{
private:
	std::unique_ptr<GL::VAO> vao;
	std::unique_ptr<GL::VBO> vbo;
	std::unique_ptr<GL::Program> program;

public:
	PointSystem(std::shared_ptr<entt::registry> registry);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;
};

