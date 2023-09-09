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
	this->registry->on_update<Position>().connect<&BezierC2System::UpdateAllBeziersContainingPoint>(*this);
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

	glm::vec3 deBoor0 = Utils::GetTranslation(registry, bezC2.deBoorPoints[0]);
	glm::vec3 deBoor1 = Utils::GetTranslation(registry, bezC2.deBoorPoints[1]);
	glm::vec3 deBoor2 = Utils::GetTranslation(registry, bezC2.deBoorPoints[2]);
	glm::vec3 deBoor3 = Utils::GetTranslation(registry, bezC2.deBoorPoints[3]);

	glm::vec3 bernst1 = deBoor1 + (deBoor2 - deBoor1) / 3.f;
	glm::vec3 bernst2 = deBoor1 + (deBoor2 - deBoor1) * 2.f / 3.f;
	glm::vec3 helpPoint1 = deBoor0 + (deBoor1 - deBoor0) * 2.f / 3.f;
	glm::vec3 bernst0 = (helpPoint1 + bernst1) / 2.f;
	glm::vec3 helpPoint2 = deBoor2 + (deBoor3 - deBoor2) / 3.f;
	glm::vec3 bernst3 = (bernst2 + helpPoint2) / 2.f;

	//this->entitiesFactory->CreatePoint(helpPoint1);
	//this->entitiesFactory->CreatePoint(helpPoint2);

	registry.patch<BezierC0>(bezierc2Entity, [&](BezierC0& bezC0) -> void
		{
			auto pointsClone = bezC0.points;
			for (auto pt : pointsClone)
			{
				registry.destroy(pt);
			}
			bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernst0));
			bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernst1));
			bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernst2));
			bezC0.points.push_back(this->entitiesFactory->CreatePoint(bernst3));
		});
}

void BezierC2System::UpdateAllBeziersContainingPoint(entt::registry& registry, entt::entity positionEntity)
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
