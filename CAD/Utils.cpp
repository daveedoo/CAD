#include "Utils.h"
#include "src\renderer\objects\Components\Transformation.h"
#include "src\maths\Matrix.h"
#include "src\renderer\objects\Components\BezierC0.h"
#include "src\renderer\objects\Components\BezierC2.h"

const glm::vec3 Utils::PolylineColor = glm::vec3(0.039f, 0.729f, 0.710f);

glm::vec3 Utils::GetScreenPositionFrom3DCoordinates(const glm::vec3& position, const Camera& camera)
{
	int scrWidth, scrHeight;
	camera.GetViewportSize(scrWidth, scrHeight);

	auto scr = camera.GetProjectionMatrix() * camera.GetViewMatrix() * glm::vec4(position, 1.f);
	scr /= scr.w;
	scr.x = ((scr.x + 1.f) / 2.f) * scrWidth;
	scr.y = ((-scr.y + 1.f) / 2.f) * scrHeight;
	return glm::vec3(scr);
}

glm::vec3 Utils::GetObjectColor(bool isSelected)
{
	return isSelected ? glm::vec3(1.f, 0.65f, 0.f) : glm::vec3(1.f, 1.f, 1.f);
}

glm::vec3 Utils::GetTranslation(const entt::registry& registry, entt::entity pointEntity)
{
	auto& transf = registry.get<Transformation>(pointEntity);
	return Matrix::ExtractTranslation(transf.worldMatrix);
}
