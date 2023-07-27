#include "TransformationsSystem.h"
#include "../../../Matrix.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Transformation.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Rotation.h"
#include "..\objects\Components\Scaling.h"

TransformationsSystem::TransformationsSystem(std::shared_ptr<entt::registry> registry) : System(registry)
{
	this->registry->on_construct<Position>().connect<&TransformationsSystem::FixTransfomationComponent_OnConstruct>(*this);
	this->registry->on_construct<ScaleRotation>().connect<&TransformationsSystem::FixTransfomationComponent_OnConstruct>(*this);
	this->registry->on_destroy<Position>().connect<&TransformationsSystem::FixTransfomationComponent_OnPositionDestroy>(*this);
	this->registry->on_destroy<ScaleRotation>().connect<&TransformationsSystem::FixTransfomationComponent_OnScaleRotationDestroy>(*this);

	this->registry->on_update<Position>().connect<&TransformationsSystem::UpdateTransformationComponent>(*this);
	this->registry->on_update<ScaleRotation>().connect<&TransformationsSystem::UpdateTransformationComponent>(*this);
}

void TransformationsSystem::Update(const Camera& camera)
{
}

void TransformationsSystem::Render(const Camera& camera)
{
}

void TransformationsSystem::FixTransfomationComponent(entt::entity entity,
	const Position* position, const ScaleRotation* scaleRot, const Transformation* transformation)
{
	if (position == nullptr && scaleRot == nullptr && transformation != nullptr)
		this->registry->remove<Transformation>(entity);
	else if ((position != nullptr || scaleRot != nullptr) && transformation == nullptr)
		this->registry->emplace<Transformation>(entity);

	UpdateTransformationComponent(*this->registry, entity);
}

void TransformationsSystem::FixTransfomationComponent_OnConstruct(entt::registry& registry, entt::entity entity)
{
	auto [position, scaleRot, transformation] = this->registry->try_get<Position, ScaleRotation, Transformation>(entity);
	FixTransfomationComponent(entity, position, scaleRot, transformation);
}

void TransformationsSystem::FixTransfomationComponent_OnPositionDestroy(entt::registry& registry, entt::entity entity)
{
	auto [scaleRot, transformation] = this->registry->try_get<ScaleRotation, Transformation>(entity);
	FixTransfomationComponent(entity, nullptr, scaleRot, transformation);
}

void TransformationsSystem::FixTransfomationComponent_OnScaleRotationDestroy(entt::registry& registry, entt::entity entity)
{
	auto [position, transformation] = this->registry->try_get<Position, Transformation>(entity);
	FixTransfomationComponent(entity, position, nullptr, transformation);
}

void TransformationsSystem::UpdateTransformationComponent(entt::registry& registry, entt::entity entity)
{
	AdditionalTransformation* groupTransf = this->registry->ctx().contains<AdditionalTransformation>() ?
		&(this->registry->ctx().get<AdditionalTransformation>()) :
		nullptr;

	auto& position = this->registry->get<Position>(entity);	// If entity contains ScaleRotation component, it also has Position.
	auto [scaleRot, selectable] = this->registry->try_get<ScaleRotation, Selectable>(entity);
	auto addTransf = (selectable != nullptr && selectable->selected) ? groupTransf : nullptr;

	this->registry->patch<Transformation>(entity, [&](Transformation& transformation) -> void
		{
			Scaling* scaling = scaleRot == nullptr ? nullptr : new Scaling(scaleRot->scale);
			Rotation* rotation = scaleRot == nullptr ? nullptr : new Rotation(scaleRot->axisFi, scaleRot->axisLambda, scaleRot->angle);
			transformation.worldMatrix = Matrix::GetResultingTransformationMatrix(position, scaling, rotation, addTransf);
			delete scaling;
			delete rotation;
		});
}
