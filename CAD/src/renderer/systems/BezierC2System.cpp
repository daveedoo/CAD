#include "BezierC2System.h"
#include "..\objects\Components\BezierC2.h"
#include "..\objects\Components\BezierC0.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\maths\Matrix.h"
#include "..\..\..\Utils.h"
#include "..\objects\Components\Point.h"
#include "BezierC0System.h"

BezierC2System::BezierC2System(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory) : System(registry),
	entitiesFactory(entitiesFactory)
{
	this->registry->on_construct<BezierC2>().connect<&BezierC2System::ReinitializeBernsteinPoints>(*this);
	this->registry->on_update<BezierC2>().connect<&BezierC2System::ReinitializeBernsteinPoints>(*this);
	this->registry->on_update<Position>().connect<&BezierC2System::UpdateAllBeziersContainingDeBoorPoint>(*this);
	this->registry->on_destroy<Point>().connect<&BezierC2System::RemovePointFromDeBoorPoints>(*this);
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
					// TODO: may be improved, just get the first BezierC0 different than 'bezC0'
					// assumption: each bernstein point belongs to the list only once
					if (Utils::GetListOfBeziersContainingPoint(registry, bernstEntitiesCopy[i]).size() > 1)
					{
						auto newEnd = std::remove(bezC0.points.begin(), bezC0.points.end(), bernstEntitiesCopy[i]);
						if (newEnd != bezC0.points.end())
						{
							bezC0.points.erase(newEnd, bezC0.points.end());
						}
					}
					else
					{
						registry.destroy(bernstEntitiesCopy[i]);
					}
				}
			}
			// some new de Boor points added (possibly)
			else
			{
				for (size_t i = pointsToPatchCount; i < bernstPositions.size(); i++)
				{
					bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernstPositions[i]));
				}
			}
		});
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
