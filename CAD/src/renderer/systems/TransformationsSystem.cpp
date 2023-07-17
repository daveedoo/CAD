#include "TransformationsSystem.h"
#include "../../../Matrix.h"
#include "..\objects\Components\Position.h"
#include "..\objects\Components\Transformation.h"
#include "..\objects\Components\Dirty.h"
#include "..\objects\Components\Selectable.h"

TransformationsSystem::TransformationsSystem(std::shared_ptr<entt::registry> registry) : System(registry)
{
	this->registry->on_construct<Position>().before().connect<&TransformationsSystem::FixTransfomationComponent>(*this);
	this->registry->on_construct<ScaleRotation>().before().connect<&TransformationsSystem::FixTransfomationComponent>(*this);
	this->registry->on_destroy<Position>().connect<&TransformationsSystem::FixTransfomationComponent>(*this);
	this->registry->on_destroy<ScaleRotation>().connect<&TransformationsSystem::FixTransfomationComponent>(*this);
}

glm::mat4 GetTransformation(const Position& position, const ScaleRotation* scaleRot, const Transformation& transformation, const AdditionalTransformation* addTransf)	// TODO: remove transformation (?)
{
	glm::mat4 worldMtx = glm::mat4(1.f);
	
	// apply translation resulting from scale
	glm::vec3 resPosition = addTransf == nullptr ?
		position.position :
		addTransf->scaleRotation.scale * (position.position - addTransf->centerPoint) + addTransf->centerPoint;
	worldMtx *= Matrix::Translation(resPosition);

	if (addTransf != nullptr)
		worldMtx *= Matrix::RotationAroundPoint(*addTransf, position.position);
	
	if (scaleRot != nullptr)
	{
		// scale
		glm::vec3 resScale = scaleRot->scale;
		if (addTransf != nullptr)
			resScale *= addTransf->scaleRotation.scale;

		worldMtx *= Matrix::Rotation(*scaleRot) * Matrix::Scale(resScale);

		//if (addTransf != nullptr)
		//{
		//	auto worldMatrix = transformation.worldMatrix;
		//	float trace = worldMatrix[0][0] + worldMatrix[1][1] + worldMatrix[2][2];
		//	float c = (trace - 1.f) / 2.f;
		//	float s = glm::sqrt(1 - c * c);
		//	float x = (worldMatrix[3][2] - worldMatrix[2][3]) / (2 * s);
		//	float y = (worldMatrix[1][3] - worldMatrix[3][1]) / (2 * s);
		//	float z = (worldMatrix[2][1] - worldMatrix[1][2]) / (2 * s);
		//	//float x = (worldMatrix[2][3] - worldMatrix[3][2]) / (2 * s);
		//	//float y = (worldMatrix[3][1] - worldMatrix[1][3]) / (2 * s);
		//	//float z = (worldMatrix[1][2] - worldMatrix[2][1]) / (2 * s);
		//	float a = glm::acos(c);
		//	float lambda = glm::atan(x / y);
		//	float fi = glm::atan(z / y * glm::sin(lambda));
		//	if (isnan(a))
		//		a = 0.f;
		//	if (isnan(lambda))
		//		lambda = 0.f;
		//	if (isnan(fi))
		//		fi = 0;
		//	resRotation = ScaleRotation(resScale, glm::degrees(fi), glm::degrees(lambda), glm::degrees(a));
		//}
	}

	return worldMtx;
}

void TransformationsSystem::Update(const Camera& camera)
{
	//FixTransfomationComponent();

	AdditionalTransformation* groupTransf = this->registry->ctx().contains<AdditionalTransformation>() ?
		&(this->registry->ctx().get<AdditionalTransformation>()) :
		nullptr;

	auto view = this->registry->view<Dirty, Position, Transformation>();
	for (auto [entity, position, transformation] : view.each())
	{
		auto [scaleRot, selectable] = this->registry->try_get<ScaleRotation, Selectable>(entity);
		auto addTransf = (selectable != nullptr && selectable->selected) ? groupTransf : nullptr;

		transformation.worldMatrix = GetTransformation(position, scaleRot, transformation, addTransf);
		this->registry->remove<Dirty>(entity);
	}
}

void TransformationsSystem::Render(const Camera& camera)
{
}

void TransformationsSystem::FixTransfomationComponent(entt::registry& registry, entt::entity entity)
{
	//auto view = this->registry->view<Dirty>();
	//for (auto [entity] : view.each())
	//{
	auto [position, scaleRot, transformation] = this->registry->try_get<Position, ScaleRotation, Transformation>(entity);

	if (position == nullptr && scaleRot == nullptr && transformation != nullptr)
		this->registry->remove<Transformation>(entity);
	else if ((position != nullptr || scaleRot != nullptr) && transformation == nullptr)
		this->registry->emplace<Transformation>(entity);
	//}
}
