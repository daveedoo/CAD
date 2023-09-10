#include "BezierC2System.h"
#include "..\objects\Components\BezierC2.h"
#include "..\objects\Components\BezierC0.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\maths\Matrix.h"
#include "..\..\..\Utils.h"
#include "..\objects\Components\Point.h"
#include "BezierC0System.h"
#include "..\objects\Components\Selectable.h"

BezierC2System::BezierC2System(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory) : System(registry),
	entitiesFactory(entitiesFactory)
{
	this->registry->on_construct<BezierC2>().connect<&BezierC2System::ReinitializeBernsteinPoints>(*this);
	this->registry->on_update<BezierC2>().connect<&BezierC2System::ReinitializeBernsteinPoints>(*this);
	this->registry->on_update<Position>().connect<&BezierC2System::UpdateAllBeziersContainingDeBoorPoint>(*this);
	this->registry->on_destroy<Point>().connect<&BezierC2System::RemovePointFromDeBoorPoints>(*this);
	
	this->registry->on_update<Position>().connect<&BezierC2System::AdjustDeBoorPointsOnBernsteinChange>(*this);
}

void BezierC2System::Update(const Camera& camera)
{
}

void BezierC2System::Render(const Camera& camera)
{
}

void BezierC2System::ReinitializeBernsteinPoints(entt::registry& registry, entt::entity bezierc2Entity)
{
	auto& bezC2 = registry.get<BezierC2>(bezierc2Entity);

	std::vector<glm::vec3> deBoorPositions = GetDeBoorPositions(registry, bezC2.deBoorPoints);
	std::vector<glm::vec3> bernstPositions = CalulateBernsteinPositions(deBoorPositions);

	this->registry->on_update<Position>().disconnect<&BezierC2System::AdjustDeBoorPointsOnBernsteinChange>(*this);
	registry.patch<BezierC0>(bezierc2Entity, [&](BezierC0& bezC0) -> void
		{
			size_t pointsToPatchCount = std::min(bernstPositions.size(), bezC0.points.size());
			for (size_t i = 0; i < pointsToPatchCount; i++)
			{
				registry.patch<Position>(bezC0.points[i], [&](Position& positionComp) -> void
					{
						positionComp.position = bernstPositions[i];
					});
			}

			// some de Boor points deleted
			if (bernstPositions.size() < bezC0.points.size())
			{
				std::vector<entt::entity> bernstEntitiesCopy = bezC0.points;
				for (size_t i = pointsToPatchCount; i < bernstEntitiesCopy.size(); i++)
				{
					registry.destroy(bernstEntitiesCopy[i]);
				}
			}
			// some new de Boor points added (possibly)
			else
			{
				for (size_t i = pointsToPatchCount; i < bernstPositions.size(); i++)
				{
					bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernstPositions[i], true));
				}
			}
		});
	this->registry->on_update<Position>().connect<&BezierC2System::AdjustDeBoorPointsOnBernsteinChange>(*this);
}

std::vector<glm::vec3> BezierC2System::GetDeBoorPositions(entt::registry& registry, std::vector<entt::entity> deBoorPointEntities)
{
	std::vector<glm::vec3> deBoorPositions;
	for (auto point : deBoorPointEntities)
	{
		deBoorPositions.push_back(Utils::GetTranslation(registry, point));
	}
	return deBoorPositions;
}

std::vector<glm::vec3> BezierC2System::CalulateBernsteinPositions(std::vector<glm::vec3> deBoorPoints)
{
	std::vector<glm::vec3> bernsteinPoints;
	if (deBoorPoints.size() < 4)
		return bernsteinPoints;

	// we go with with 3 consecutive de Boor points each loop iteration
	// index i represents the index of the "center" point
	for (size_t i = 1; i < deBoorPoints.size() - 1; i++)
	{
		glm::vec3 deBoorPrev = deBoorPoints[i - 1];
		glm::vec3 deBoorCent = deBoorPoints[i];
		glm::vec3 deBoorNext = deBoorPoints[i + 1];

		glm::vec3 bernstPrev = deBoorCent + (deBoorPrev - deBoorCent) / 3.f;
		glm::vec3 bernstNext = deBoorCent + (deBoorNext - deBoorCent) / 3.f;
		glm::vec3 bernstCent = (bernstPrev + bernstNext) / 2.f;

		if (i > 1)
		{
			bernsteinPoints.push_back(bernstPrev);
		}
		bernsteinPoints.push_back(bernstCent);
		if (i < deBoorPoints.size() - 2)
		{
			bernsteinPoints.push_back(bernstNext);
		}
	}

	return bernsteinPoints;
}

void BezierC2System::UpdateAllBeziersContainingDeBoorPoint(entt::registry& registry, entt::entity positionEntity)
{
	if (!registry.all_of<Point>(positionEntity))
		return;
	auto view = this->registry->view<BezierC2>();
	for (auto [bezierEntity, bezierC2] : view.each())
	{
		if (std::find(bezierC2.deBoorPoints.begin(), bezierC2.deBoorPoints.end(), positionEntity) != bezierC2.deBoorPoints.end())
			ReinitializeBernsteinPoints(registry, bezierEntity);
	}
}

void BezierC2System::RemovePointFromDeBoorPoints(entt::registry& registry, entt::entity pointEntity)
{
	auto view = registry.view<BezierC2>();
	for (auto [bezierEntity, bezier] : view.each())
	{
		auto newEnd = std::remove(bezier.deBoorPoints.begin(), bezier.deBoorPoints.end(), pointEntity);
		if (newEnd != bezier.deBoorPoints.end())
		{
			bezier.deBoorPoints.erase(newEnd, bezier.deBoorPoints.end());
			ReinitializeBernsteinPoints(registry, bezierEntity);
		}
	}
}

// returns [BezierC0 entity, index of the point]
std::optional<std::tuple<entt::entity, size_t>> FindBezierC0EntityContainingPoint(entt::registry& registry, entt::entity pointEntity)
{
	auto view = registry.view<BezierC0>();
	for (auto [bezierEntity, bezier] : view.each())
	{
		auto it = std::find(bezier.points.begin(), bezier.points.end(), pointEntity);
		if (it != bezier.points.end())
			return std::make_tuple(bezierEntity, std::distance(bezier.points.begin(), it));
	}
	return std::nullopt;
}

void BezierC2System::AdjustDeBoorPointsOnBernsteinChange(entt::registry& registry, entt::entity positionEntity)
{
	if (!registry.all_of<Point>(positionEntity) || registry.all_of<Selectable>(positionEntity))
		return;
	
	auto foundIdx = FindBezierC0EntityContainingPoint(registry, positionEntity);
	if (!foundIdx.has_value())
		return;

	auto& [bezC0Entity, idxInC0] = foundIdx.value();
	if (!registry.all_of<BezierC2>(bezC0Entity))
		return;


	auto& pointPos = registry.get<Position>(positionEntity);
	auto [bezC0, bezC2] = registry.get<BezierC0, BezierC2>(bezC0Entity);

	registry.on_update<Position>().disconnect<&BezierC2System::AdjustDeBoorPointsOnBernsteinChange>(*this);
	if (idxInC0 % 3 == 0)
	{
		auto& B1 = pointPos.position;
		auto deB1 = bezC2.deBoorPoints[idxInC0 / 3 + 1];
		auto& deB0Pos = registry.get<Position>(bezC2.deBoorPoints[idxInC0 / 3 + 0]).position;
		auto& deB2Pos = registry.get<Position>(bezC2.deBoorPoints[idxInC0 / 3 + 2]).position;

		registry.patch<Position>(deB1, [&](Position& deB1Pos) -> void
			{
				deB1Pos.position = (6.f * B1 - deB0Pos - deB2Pos) / 4.f;
			});
	}
	else if (idxInC0 % 3 == 1)
	{
		auto& B1 = registry.get<Position>(bezC0.points[idxInC0 - 1]).position;
		auto newB01_2 = B1 + (B1 - pointPos.position);

		auto& deB1Pos = registry.get<Position>(bezC2.deBoorPoints[idxInC0 / 3 + 1]).position;
		auto& deB0 = bezC2.deBoorPoints[idxInC0 / 3 + 0];
		auto& deB2 = bezC2.deBoorPoints[idxInC0 / 3 + 2];

		registry.patch<Position>(deB2, [&](Position& deB2Pos) -> void
			{
				deB2Pos.position = deB1Pos + 3.f * (pointPos.position - deB1Pos);
			});
		registry.patch<Position>(deB0, [&](Position& deB0Pos) -> void
			{
				deB0Pos.position = deB1Pos + 3.f * (newB01_2 - deB1Pos);
			});
	}
	else // idxInC0 % 3 == 2
	{
		auto& B1 = registry.get<Position>(bezC0.points[idxInC0 + 1]).position;
		auto newB12_1 = B1 + (B1 - pointPos.position);

		auto& deB1Pos = registry.get<Position>(bezC2.deBoorPoints[idxInC0 / 3 + 2]).position;
		auto& deB0 = bezC2.deBoorPoints[idxInC0 / 3 + 1];
		auto& deB2 = bezC2.deBoorPoints[idxInC0 / 3 + 3];

		registry.patch<Position>(deB0, [&](Position& deB0Pos) -> void
			{
				deB0Pos.position = deB1Pos + 3.f * (pointPos.position - deB1Pos);
			});
		registry.patch<Position>(deB2, [&](Position& deB2Pos) -> void
			{
				deB2Pos.position = deB1Pos + 3.f * (newB12_1 - deB1Pos);
			});
	}
	registry.on_update<Position>().connect<&BezierC2System::AdjustDeBoorPointsOnBernsteinChange>(*this);
}
