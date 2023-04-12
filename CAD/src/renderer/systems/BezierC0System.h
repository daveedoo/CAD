#pragma once
#include "System.h"
#include "../../gl/wrappers/Program.h"
#include "../../Window/input/handlers/CameraMovementInputHandler.h"

class BezierC0System : public System
{
private:
	std::unique_ptr<GL::Program> program;
	std::shared_ptr<CameraMovementInputHandler> cameraHandler;
	std::shared_ptr<Camera> camera;

	int scrWidth = 1024;
	int scrHeight = 720;
	double segmentsMultiplier = 1.0;

public:
	virtual void Update(const Camera& camera) override;
	virtual void Render(const Camera& camera) override;

	BezierC0System(std::shared_ptr<entt::registry> registry, std::shared_ptr<CameraMovementInputHandler> cameraHandler, std::shared_ptr<Camera> camera);
	void UpdateScreenSize(int width, int height);

private:
	static glm::vec3 CalculateBezierValue(const glm::vec3& b0, const glm::vec3& b1, const glm::vec3& b2, const glm::vec3& b3, float t);
	std::vector<glm::vec3> CalculateBezierValues(const BezierC0& bezier);
	unsigned int CalculateSegmentsCount(std::array<glm::vec3, 4> arr);
	void UpdateCurveMesh(entt::registry& registry, entt::entity entity);

};
