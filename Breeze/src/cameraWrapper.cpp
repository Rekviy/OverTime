#include "cameraWrapper.h"
using namespace overtime;

static const float DESIGN_W = 1280.0f, DESIGN_H = 720.0f;
static const float ASPECT_RATIO = DESIGN_W / DESIGN_H;

orthographCamera cameraWrapper::_camera = orthographCamera(-DESIGN_W / 2.0f, DESIGN_W / 2.0f, -DESIGN_H / 2.0, DESIGN_H / 2.0f);

void cameraWrapper::updateCamera(float newWidth, float newHeight)
{
	float windowAspect = newWidth / newHeight;

	float scaleX = 1.0f, scaleY = 1.0f;

	(windowAspect > ASPECT_RATIO) ? scaleX = windowAspect / ASPECT_RATIO : scaleY = ASPECT_RATIO / windowAspect;
	_camera.setProjectionMatrix(-DESIGN_W / 2.0f * scaleX, DESIGN_W / 2.0f * scaleX, -DESIGN_H / 2.0f * scaleY, DESIGN_H / 2.0f * scaleY);
}

glm::vec2 cameraWrapper::worldToScreen(const glm::vec2& worldPos)
{
	glm::vec4 clipSpacePos = _camera.getPVMatrix() * glm::vec4(worldPos.x, worldPos.y, 0.0f, 1.0f);
	return {
		(clipSpacePos.x / clipSpacePos.w + 1.0f) / 2.0f * application::getInst().getWindow().getWidth(),
		(1.0f - (clipSpacePos.y / clipSpacePos.w + 1.0f) / 2.0f) * application::getInst().getWindow().getHeight()
	};
}
glm::vec2 cameraWrapper::screenToWorld(const glm::vec2& screenPos)
{
	auto& app = application::getInst();
	glm::vec2 normalizedPos = glm::vec2(
		screenPos.x / app.getWindow().getWidth() * 2.0f - 1.0f,
		1.0f - screenPos.y / app.getWindow().getHeight() * 2.0f
	);

	glm::vec4 worldPos = glm::inverse(_camera.getPVMatrix()) * glm::vec4(normalizedPos.x, normalizedPos.y, 0.0f, 1.0f);

	return glm::vec2(worldPos.x, worldPos.y);
}

