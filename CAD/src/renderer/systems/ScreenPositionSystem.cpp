#include "ScreenPositionSystem.h"
#include "../../Config.h"

ScreenPositionSystem::ScreenPositionSystem(std::shared_ptr<entt::registry> registry, Camera& camera) : System(registry),
	camera(camera)
{
	this->registry->on_construct<Position>().connect<&ScreenPositionSystem::AddScreenPositionComponent>(*this);
	this->registry->on_destroy<Position>().connect<&ScreenPositionSystem::RemoveScreenPositionComponent>(*this);
	
	this->registry->on_update<Position>().connect<&ScreenPositionSystem::SetPositionScreenBasedOn3D>(*this);
	this->registry->on_update<Selectable>().connect<&ScreenPositionSystem::SetPositionScreenBasedOn3D>(*this);
}

void ScreenPositionSystem::Update(const Camera& camera)
{
}

void ScreenPositionSystem::Render(const Camera& camera)
{
}

// Updates only selected OR unselectable entities (like main cursor)
// Unselected entities don't show their screen coordinates, so there's no need to update it.
void ScreenPositionSystem::SetPositionScreenBasedOn3D(entt::registry& registry, entt::entity entity)
{
	const auto& selectable = registry.try_get<Selectable>(entity);
	if (selectable != nullptr && selectable->selected == false)
		return;

	const auto& pos3d = registry.get<Position>(entity);
	registry.patch<ScreenPosition>(entity, [&](ScreenPosition& screenPosition) -> void
		{
			auto scr = camera.GetProjectionMatrix() * camera.GetViewMatrix() * glm::vec4(pos3d.position, 1.f);
			scr /= scr.w;
			screenPosition.position = glm::vec3(scr);
		});
}

void ScreenPositionSystem::AddScreenPositionComponent(entt::registry& registry, entt::entity entity)
{
	registry.emplace<ScreenPosition>(entity);
}

void ScreenPositionSystem::RemoveScreenPositionComponent(entt::registry& registry, entt::entity entity)
{
	registry.remove<ScreenPosition>(entity);
}

void ScreenPositionSystem::OnScreenSizeChanged()
{
	auto view = this->registry->view<Position>();
	for (auto [entity, position] : view.each())
	{
		SetPositionScreenBasedOn3D(*this->registry, entity);
	}
}
