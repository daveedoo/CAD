#include "MouseSelectionSystem.h"
#include "..\..\Window\input\events\modded\MouseClickEvent.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Selectable.h"
#include "../../../Utils.h"
#include <tuple>

MouseSelectionSystem::MouseSelectionSystem(std::shared_ptr<entt::registry> registry, std::shared_ptr<Camera> camera) : System(registry),
	camera(camera)
{
}

void MouseSelectionSystem::Update(const Camera& camera)
{
}

void MouseSelectionSystem::Render(const Camera& camera)
{
}

void MouseSelectionSystem::ProcessInput(const InputEvent& event)
{
	static const float MAX_SCREEN_DIFFERENCE = 5.f;	// px

	if (event.type == InputEvent::EventType::MOUSE_CLICK)
	{
		const auto& mouseEvent = static_cast<const MouseClickEvent&>(event);
		if (mouseEvent.action == KeyOrButtonEvent::Action::RELEASE &&
			mouseEvent.button == MouseClickEvent::Button::LEFT)
		{
			std::vector<std::tuple<entt::entity, float>> entities;

			auto view = this->registry->view<Position, Selectable>();
			for (auto [entity, position, selectable] : view.each())
			{
				glm::vec3 screenPos = Utils::GetScreenPositionFrom3DCoordinates(position.position, *this->camera);
				if (glm::distance(glm::vec2(mouseEvent.xpos, mouseEvent.ypos), glm::vec2(screenPos.x, screenPos.y)) < MAX_SCREEN_DIFFERENCE)
				{
					entities.push_back(std::make_tuple(entity, screenPos.z));
				}
			}

			if (entities.size() > 0)
			{
				auto& [minZEntity, minZVal] = *std::min_element(entities.begin(), entities.end(),
					[](std::tuple<entt::entity, float>& ent1, std::tuple<entt::entity, float>& ent2) -> bool
					{
						auto& [entity1, z1] = ent1;
						auto& [entity2, z2] = ent2;
						return z1 < z2;
					});

				this->registry->patch<Selectable>(minZEntity, [](Selectable& selectable) -> void
					{
						selectable.selected = !selectable.selected;
					});
			}
			else
			{
				for (auto [entity, position, selectable] : view.each())
				{
					if (selectable.selected)
					{
						this->registry->patch<Selectable>(entity, [](Selectable& selectable) -> void
							{
								selectable.selected = false;
							});
					}
				}
			}
		}
	}
}
