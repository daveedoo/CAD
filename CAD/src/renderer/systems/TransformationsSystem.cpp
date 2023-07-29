#include "TransformationsSystem.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Transformation.h"
#include "..\objects\Components\Selectable.h"
#include "..\objects\Components\Rotation.h"
#include "..\objects\Components\Scaling.h"
#include "..\..\maths\Matrix.h"

TransformationsSystem::TransformationsSystem(std::shared_ptr<entt::registry> registry) : System(registry)
{
	this->registry->on_construct<Position>().connect<&TransformationsSystem::FixTransfomationComponent_OnConstruct>(*this);
	this->registry->on_construct<Scaling>().connect<&TransformationsSystem::FixTransfomationComponent_OnConstruct>(*this);
	this->registry->on_construct<Rotation>().connect<&TransformationsSystem::FixTransfomationComponent_OnConstruct>(*this);
	
	// on_destroy event is called before actual destruction so separate callbacks are needed
	this->registry->on_destroy<Position>().connect<&TransformationsSystem::FixTransfomationComponent_OnPositionDestroy>(*this);
	this->registry->on_destroy<Scaling>().connect<&TransformationsSystem::FixTransfomationComponent_OnScalingDestroy>(*this);
	this->registry->on_destroy<Rotation>().connect<&TransformationsSystem::FixTransfomationComponent_OnScalingDestroy>(*this);

	this->registry->on_update<Position>().connect<&TransformationsSystem::UpdateTransformationComponent>(*this);
	this->registry->on_update<Scaling>().connect<&TransformationsSystem::UpdateTransformationComponent>(*this);
	this->registry->on_update<Rotation>().connect<&TransformationsSystem::UpdateTransformationComponent>(*this);
}

void TransformationsSystem::Update(const Camera& camera)
{
}

void TransformationsSystem::Render(const Camera& camera)
{
}

void TransformationsSystem::FixTransfomationComponent(entt::entity entity,
	const Position* position, const Scaling* scaling, const Rotation* rotation, const Transformation* transformation)
{
	if (position == nullptr && scaling == nullptr && rotation == nullptr && transformation != nullptr)
	{
		this->registry->remove<Transformation>(entity);
		return;
	}

	if ((position != nullptr || scaling != nullptr || rotation != nullptr) && transformation == nullptr)
		this->registry->emplace<Transformation>(entity);

	UpdateTransformationComponent(*this->registry, entity);
}

void TransformationsSystem::FixTransfomationComponent_OnConstruct(entt::registry& registry, entt::entity entity)
{
	auto [position, scaling, rotation, transformation] = this->registry->try_get<Position, Scaling, Rotation, Transformation>(entity);
	FixTransfomationComponent(entity, position, scaling, rotation, transformation);
}

void TransformationsSystem::FixTransfomationComponent_OnPositionDestroy(entt::registry& registry, entt::entity entity)
{
	auto [scaling, rotation, transformation] = this->registry->try_get<Scaling, Rotation, Transformation>(entity);
	FixTransfomationComponent(entity, nullptr, scaling, rotation, transformation);
}

void TransformationsSystem::FixTransfomationComponent_OnScalingDestroy(entt::registry& registry, entt::entity entity)
{
	auto [position, rotation, transformation] = this->registry->try_get<Position, Rotation, Transformation>(entity);
	FixTransfomationComponent(entity, position, nullptr, rotation, transformation);
}

void TransformationsSystem::FixTransfomationComponent_OnRotationDestroy(entt::registry& registry, entt::entity entity)
{
	auto [position, scaling, transformation] = this->registry->try_get<Position, Scaling, Transformation>(entity);
	FixTransfomationComponent(entity, position, scaling, nullptr, transformation);
}

void TransformationsSystem::UpdateTransformationComponent(entt::registry& registry, entt::entity entity)
{
	AdditionalTransformation* groupTransf = this->registry->ctx().contains<AdditionalTransformation>() ?
		&(this->registry->ctx().get<AdditionalTransformation>()) :
		nullptr;

	auto& position = this->registry->get<Position>(entity);	// If entity contains Scaling or Rotation component, it also has Position.
	auto [scaling, rotation, selectable] = this->registry->try_get<Scaling, Rotation, Selectable>(entity);
	auto addTransf = (selectable != nullptr && selectable->selected) ? groupTransf : nullptr;

	this->registry->patch<Transformation>(entity, [&](Transformation& transformation) -> void
		{
			transformation.worldMatrix = Matrix::GetResultingTransformationMatrix(position, scaling, rotation, addTransf);
		});
}
