#include "CancelGroupTransformation.h"
#include "..\objects\Components\AdditionalTransformation.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"

CancelGroupTransformation::CancelGroupTransformation(std::shared_ptr<entt::registry> registry,
	std::shared_ptr<AdditionalTransformation> additionalTransformation) :
	Command(), registry(registry), additionalTransformation(std::move(additionalTransformation))
{
}

void CancelGroupTransformation::execute()
{
	this->registry->ctx().erase<AdditionalTransformation>();
	*this->additionalTransformation = AdditionalTransformation(glm::vec3(0.f), 1.f);

	auto view = this->registry->view<Selectable>();
	for (auto [entity, selectable] : view.each())
	{
		if (selectable.selected)
			this->registry->patch<Position>(entity);
	}
}
