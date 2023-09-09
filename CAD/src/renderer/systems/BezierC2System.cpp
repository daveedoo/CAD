#include "BezierC2System.h"
#include "..\objects\Components\BezierC2.h"
#include "..\objects\Components\BezierC0.h"
#include "..\objects\Components\Transformation.h"
#include "..\..\maths\Matrix.h"
#include "..\..\..\Utils.h"
#include "..\objects\Components\Point.h"

BezierC2System::BezierC2System(std::shared_ptr<entt::registry> registry, std::shared_ptr<EntitiesFactory> entitiesFactory) : System(registry),
	entitiesFactory(entitiesFactory)
{
	this->registry->on_construct<BezierC2>().connect<&BezierC2System::SetBernsteinPoints>(*this);
	this->registry->on_update<BezierC2>().connect<&BezierC2System::SetBernsteinPoints>(*this);
	this->registry->on_update<Position>().connect<&BezierC2System::UpdateAllBeziersContainingDeBoorPoint>(*this);
}

void BezierC2System::Update(const Camera& camera)
{
}

void BezierC2System::Render(const Camera& camera)
{
}

void BezierC2System::SetBernsteinPoints(entt::registry& registry, entt::entity bezierc2Entity)
{
	auto& bezC2 = registry.get<BezierC2>(bezierc2Entity);
	if (bezC2.deBoorPoints.size() < 4)
		return;

	std::vector<glm::vec3> bernsteinPoints;
	// we go with with 3 consecutive de Boor points each loop iteration
	// index i represents the index of the "center" point
	for (size_t i = 1; i < bezC2.deBoorPoints.size() - 1; i++)
	{
		glm::vec3 deBoorPrev = Utils::GetTranslation(registry, bezC2.deBoorPoints[i - 1]);
		glm::vec3 deBoorCent = Utils::GetTranslation(registry, bezC2.deBoorPoints[i]);
		glm::vec3 deBoorNext = Utils::GetTranslation(registry, bezC2.deBoorPoints[i + 1]);

		glm::vec3 bernstPrev = deBoorCent + (deBoorPrev - deBoorCent) / 3.f;
		glm::vec3 bernstNext = deBoorCent + (deBoorNext - deBoorCent) / 3.f;
		glm::vec3 bernstCent = (bernstPrev + bernstNext) / 2.f;

		if (i > 1)
		{
			bernsteinPoints.push_back(bernstPrev);
		}
		bernsteinPoints.push_back(bernstCent);
		if (i < bezC2.deBoorPoints.size() - 2)
		{
			bernsteinPoints.push_back(bernstNext);
		}
	}

	registry.patch<BezierC0>(bezierc2Entity, [&](BezierC0& bezC0) -> void
		{
			size_t pointsToPatch = std::min(bernsteinPoints.size(), bezC0.points.size());
			size_t i = 0;
			for (; i < pointsToPatch; i++)
			{
				registry.patch<Position>(bezC0.points[i], [&](Position& positionComp) -> void
					{
						positionComp.position = bernsteinPoints[i];
					});
			}

			if (bernsteinPoints.size() > bezC0.points.size())
			{
				for (; i < bernsteinPoints.size(); i++)
				{
					bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernsteinPoints[i]));
				}
			}
		});
}

void BezierC2System::UpdateAllBeziersContainingDeBoorPoint(entt::registry& registry, entt::entity positionEntity)
{
	if (!registry.all_of<Point>(positionEntity))
		return;
	auto view = this->registry->view<BezierC2>();
	for (auto [bezierEntity, bezierC2] : view.each())
	{
		if (std::find(bezierC2.deBoorPoints.begin(), bezierC2.deBoorPoints.end(), positionEntity) != bezierC2.deBoorPoints.end())
			SetBernsteinPoints(registry, bezierEntity);
	}
}
