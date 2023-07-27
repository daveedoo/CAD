#include "ApplyGroupTransformation.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\..\Matrix.h"
#include <glm\gtx\matrix_decompose.hpp>
#include "..\..\maths\RotationRepresentationsConverter.h"

ApplyGroupTransformation::ApplyGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<ScaleRotation> scaleRotation)
	: registry(registry),
	scaleRotation(scaleRotation)
{
}

void ApplyGroupTransformation::execute()
{
	this->registry->ctx().erase<AdditionalTransformation>();

	auto view = this->registry->view<Selectable, Transformation>();
	for (auto [entity, selectable, transformation] : view.each())
	{
		if (selectable.selected)
		{
			glm::vec3 scale, translation; glm::quat q;
			Matrix::Decompose(transformation.worldMatrix, scale, q, translation);
			
			this->registry->patch<Position>(entity, [&](Position& position) -> void
				{
					position.position = translation;
				});

			if (this->registry->all_of<ScaleRotation>(entity))
			{
				this->registry->patch<ScaleRotation>(entity, [&](ScaleRotation& scaleRot) -> void
					{
						scaleRot = RotationRepresentationsConverter::ConvertToAxisAngle(q);
						scaleRot.scale = scale;
					});
			}
		}
	}

	*this->scaleRotation = ScaleRotation();
}
