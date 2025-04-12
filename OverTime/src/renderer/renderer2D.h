#pragma once
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "camera.h"

#include <glm/glm.hpp>

namespace overtime {

	class renderer2D {
	public:
		struct storage;
		static void init();
		static void shutdown();

		static void beginScene(const orthographCamera& camera);
		static void endScene();

		static void drawSquad(const glm::vec4& color, const glm::vec2& pos, const glm::vec2& size, float rotation = 0.0f);
		static void drawSquad(const glm::vec4& color, const glm::vec3& pos, const glm::vec2& size, float rotation = 0.0f);
	};
}

#endif