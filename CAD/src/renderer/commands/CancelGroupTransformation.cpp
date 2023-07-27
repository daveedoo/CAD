#include "CancelGroupTransformation.h"
#include "..\objects\Components\AdditionalTransformation.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"

CancelGroupTransformation::CancelGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<ScaleRotation> scaleRotation) :
	Command(), registry(registry), scaleRotation(std::move(scaleRotation))
{
}

void CancelGroupTransformation::execute()
{
	this->registry->ctx().erase<AdditionalTransformation>();
	*this->scaleRotation = ScaleRotation();

	auto view = this->registry->view<Selectable>();
	for (auto [entity, selectable] : view.each())
	{
		if (selectable.selected)
			this->registry->patch<Position>(entity);
	}
}
