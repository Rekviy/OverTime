#pragma once
#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

#include <overtime.h>
namespace breeze {
	class cameraWrapper {
	public:
		inline static const overtime::orthographCamera& getCamera() { return _camera; }
		static void updateCamera(float newWidth, float newHeight);
		static glm::vec2 worldToScreen(const glm::vec2& worldPos);
		static glm::vec2 screenToWorld(const glm::vec2& screenPos);

	private:
		static overtime::orthographCamera _camera;
	};
}
#endif