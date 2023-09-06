#include "SortingSystem.h"
#include "..\objects\Components\Selectable.h"

SortingSystem::SortingSystem(std::shared_ptr<entt::registry> registry) : System(std::move(registry))
{
	//this->registry->on_construct<Selectable>().connect<&SortingSystem::SortSelectables>(*this);
	//this->registry->on_destroy<Selectable>().connect<&SortingSystem::SortSelectables>(*this);
}

void SortingSystem::Update(const Camera& camera)
{
}

void SortingSystem::Render(const Camera& camera)
{
}

void SortingSystem::SortSelectables()
{
	//this->registry->sort<Selectable>([](entt::entity entity1, entt::entity entity2) -> bool
	//	{
	//		return static_cast<uint64_t>(entity1) < static_cast<uint64_t>(entity2);
	//	});
	this->registry->sort<Selectable>([](const Selectable& selectable1, const Selectable& selectable2) -> bool
		{
			return selectable1.name.compare(selectable2.name) < 0;
		});
}
