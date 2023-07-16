#include "ScreenPositionSystem.h"
#include "../../Config.h"
#include <iostream>

ScreenPositionSystem::ScreenPositionSystem(std::shared_ptr<entt::registry> registry,
	Camera& camera, unsigned int screenWidth, unsigned int screenHeight)
	: System(registry),
	  camera(camera),
	  screenWidth(screenWidth),
	  screenHeight(screenHeight)
{
	this->registry->on_construct<Position>().connect<&ScreenPositionSystem::AddScreenPositionComponent>(*this);
	this->registry->on_destroy<Position>().connect<&ScreenPositionSystem::RemoveScreenPositionComponent>(*this);
	
	this->registry->on_update<Position>().connect<&ScreenPositionSystem::SetPosition_ScreenBasedOn3D>(*this);
	this->registry->on_update<Selectable>().connect<&ScreenPositionSystem::SetPosition_ScreenBasedOn3D>(*this);

	this->registry->on_update<ScreenPosition>().connect<&ScreenPositionSystem::SetPosition_3DBasedOnScreen>(*this);
}

void ScreenPositionSystem::Update(const Camera& camera)
{
}

void ScreenPositionSystem::Render(const Camera& camera)
{
}

// Updates only selected OR unselectable entities (like main cursor)
// Unselected entities don't show their screen coordinates, so there's no need to update it.
void ScreenPositionSystem::SetPosition_ScreenBasedOn3D(entt::registry& registry, entt::entity entity)
{
	const auto& selectable = registry.try_get<Selectable>(entity);
	if (selectable != nullptr && selectable->selected == false)
		return;

	this->registry->on_update<ScreenPosition>().disconnect<&ScreenPositionSystem::SetPosition_3DBasedOnScreen>(*this);

	const auto& pos3d = registry.get<Position>(entity);
	registry.patch<ScreenPosition>(entity, [&](ScreenPosition& screenPosition) -> void
		{
			auto scr = camera.GetProjectionMatrix() * camera.GetViewMatrix() * glm::vec4(pos3d.position, 1.f);
			scr /= scr.w;
			scr.x = ((scr.x + 1.f) / 2.f) * screenWidth;
			scr.y = ((-scr.y + 1.f) / 2.f) * screenHeight;
			screenPosition.position = glm::vec3(scr);
		});

	this->registry->on_update<ScreenPosition>().connect<&ScreenPositionSystem::SetPosition_3DBasedOnScreen>(*this);
}

void ScreenPositionSystem::SetPosition_3DBasedOnScreen(entt::registry& registry, entt::entity entity)
{
	registry.on_update<Position>().disconnect<&ScreenPositionSystem::SetPosition_ScreenBasedOn3D>(*this);

	const auto& screenPos = registry.get<ScreenPosition>(entity);
	registry.patch<Position>(entity, [&](Position& position) -> void
		{
			auto scr = glm::vec4(screenPos.position, 1.f);
			scr.x = ((scr.x / screenWidth) * 2.f) - 1.f;
			scr.y = ((scr.y / screenHeight) * -2.f) + 1.f;

			glm::vec4 pos3d = glm::inverse(camera.GetViewMatrix()) * glm::inverse(camera.GetProjectionMatrix()) * scr;
			pos3d /= pos3d.w;
			position.position = glm::vec3(pos3d);
		});

	registry.on_update<Position>().connect<&ScreenPositionSystem::SetPosition_ScreenBasedOn3D>(*this);
}

void ScreenPositionSystem::AddScreenPositionComponent(entt::registry& registry, entt::entity entity)
{
	registry.emplace<ScreenPosition>(entity);
}

void ScreenPositionSystem::RemoveScreenPositionComponent(entt::registry& registry, entt::entity entity)
{
	registry.remove<ScreenPosition>(entity);
}

void ScreenPositionSystem::OnScreenSizeChanged(unsigned int width, unsigned int height)
{
	this->screenWidth = width;
	this->screenHeight = height;

	auto view = this->registry->view<Position>();
	for (auto [entity, position] : view.each())
	{
		SetPosition_ScreenBasedOn3D(*this->registry, entity);
	}
}
