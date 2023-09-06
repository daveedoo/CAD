#pragma once
#include "System.h"
#include "../../gl/wrappers/Program.h"
#include "..\objects\Components\Mesh.h"

class CursorSystem : public System
{
private:
	std::unique_ptr<GL::Program> program;
	std::unique_ptr<Mesh> cursorMesh;

public:
	CursorSystem(std::shared_ptr<entt::registry> registry);

	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;
};

