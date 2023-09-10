#include "AddPointCommand.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\BezierC0.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\BezierC2.h"

AddPointCommand::AddPointCommand(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory, entt::entity mainCursor)
	: registry(std::move(registry)), entitiesFactory(std::move(entitiesFactory)), mainCursor(mainCursor)
{
}

void AddPointCommand::execute()
{
	auto& cursorPos = this->registry->get<Position>(this->mainCursor).position;
	entt::entity createdPoint = entitiesFactory->CreatePoint(cursorPos);

	auto viewC0 = this->registry->view<BezierC0, Selectable>();
	for (auto [entity, bezierc0, selectable] : viewC0.each())
	{
		if (selectable.selected)
		{
			if (this->registry->all_of<BezierC2>(entity))
			{
				this->registry->patch<BezierC2>(entity, [&](BezierC2& bezier) -> void
					{
						bezier.deBoorPoints.push_back(createdPoint);
					});
			}
			else
			{
				this->registry->patch<BezierC0>(entity, [&](BezierC0& bezier) -> void
					{
						bezier.points.push_back(createdPoint);
					});
			}
		}
	}
}
