#include "ApplyGroupTransformation.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Transformation.h"
#include <glm\gtx\matrix_decompose.hpp>
#include "..\..\maths\RotationRepresentationsConverter.h"
#include "..\..\maths\Matrix.h"

ApplyGroupTransformation::ApplyGroupTransformation(std::shared_ptr<entt::registry> registry, std::shared_ptr<AdditionalTransformation> additionalTransformation)
	: registry(registry),
	additionalTransformation(additionalTransformation)
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

			if (this->registry->all_of<Scaling>(entity))
			{
				this->registry->patch<Scaling>(entity, [&](Scaling& scaling) -> void
					{
						scaling.scale = scale;
					});
			}
			if (this->registry->all_of<Rotation>(entity))
			{
				this->registry->patch<Rotation>(entity, [&](Rotation& scaleRot) -> void
					{
						auto angles = RotationRepresentationsConverter::ConvertToRPY(q);
						scaleRot = Rotation(glm::degrees(angles.roll), glm::degrees(angles.pitch), glm::degrees(angles.yaw));
					});
			}
		}
	}

	*this->additionalTransformation = AdditionalTransformation(glm::vec3(0.f), 1.f);
}
