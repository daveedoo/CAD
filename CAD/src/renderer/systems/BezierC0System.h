#pragma once
#include "System.h"
#include "../../gl/wrappers/Program.h"

class BezierC0System : public System
{
private:
	std::unique_ptr<GL::Program> program;

public:
	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	BezierC0System(std::shared_ptr<entt::registry> registry);
private:

	static glm::vec3 CalculateBezierValue(const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3, float t);
	std::vector<glm::vec3> CalculateBezierValues(const BezierC0& bezier, unsigned int segments);
	unsigned int CalculateSegmentsCount();
	void OnBezierC0Construct(entt::registry& registry, entt::entity entity);
};

