#include "MouseSelectionSystem.h"
#include "..\..\Window\input\events\modded\MouseClickEvent.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Selectable.h"
#include "../../../Utils.h"
#include <tuple>
#include "..\..\Window\input\events\MouseMoveEvent.h"
#include "..\objects\Components\ScreenPosition.h"

MouseSelectionSystem::MouseSelectionSystem(std::shared_ptr<entt::registry> registry, std::shared_ptr<Camera> camera, std::shared_ptr<Window> window)
	: System(registry),
	camera(camera),
	window(window),
	movedEntity(std::nullopt)
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
	static const float MAX_SCREEN_DIFFERENCE = 8.f;	// px

	if (event.type == InputEvent::EventType::MOUSE_MOVE)
	{
		const auto& mouseEvent = static_cast<const MouseMoveEvent&>(event);

		if (this->movedEntity.has_value())
		{
			this->registry->patch<ScreenPosition>(this->movedEntity.value(), [&](ScreenPosition& screenPos) -> void
				{
					screenPos.position.x = mouseEvent.xpos;
					screenPos.position.y = mouseEvent.ypos;
				});
		}
		else
		{
			bool hoveringOverEntity = false;
			auto view = this->registry->view<Position, Selectable>();
			for (auto [entity, position, selectable] : view.each())
			{
				glm::vec3 screenPos = Utils::GetScreenPositionFrom3DCoordinates(position.position, *this->camera);
				if (glm::distance(glm::vec2(mouseEvent.xpos, mouseEvent.ypos), glm::vec2(screenPos.x, screenPos.y)) < MAX_SCREEN_DIFFERENCE)
				{
					this->window->SetCursor(Window::CursorType::Hand);
					hoveringOverEntity = true;
					break;
				}
			}
			if (!hoveringOverEntity)
			{
				this->window->SetCursor(Window::CursorType::Normal);
			}
		}
	}
	else if (event.type == InputEvent::EventType::MOUSE_CLICK)
	{
		const auto& mouseEvent = static_cast<const MouseClickEvent&>(event);
		if (mouseEvent.button == MouseClickEvent::Button::LEFT)
		{
			if (mouseEvent.action == KeyOrButtonEvent::Action::PRESS)
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
					this->movedEntity = minZEntity;
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
			else if (mouseEvent.action == KeyOrButtonEvent::Action::RELEASE)
			{
				this->movedEntity = std::nullopt;
			}
		}
	}
}
