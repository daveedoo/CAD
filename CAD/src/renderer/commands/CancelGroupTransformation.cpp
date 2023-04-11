#include "CancelGroupTransformation.h"
#include "../objects/Component.h"

CancelGroupTransformation::CancelGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<ScaleRotation> scaleRotation) :
	Command(), registry(registry), scaleRotation(std::move(scaleRotation))
{
}

void CancelGroupTransformation::execute()
{
	this->registry->ctx().erase<AdditionalTransformation>();
	*this->scaleRotation = ScaleRotation();

	auto view = this->registry->view<Selectable, Position>();
	for (auto [entity, selectable, position] : view.each())
	{
		if (selectable.selected)
		{
			this->registry->emplace<Dirty>(entity);
		}
	}
}
