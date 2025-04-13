#pragma once
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include "camera.h"
#include "texture.h"
#include <glm/glm.hpp>

namespace overtime {

	class renderer2D {
	public:
		struct storage;
		static void init();
		static void shutdown();

		static void beginScene(const orthographCamera& camera);
		static void endScene();

		static void drawSquad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void drawSquad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
		static void drawSquad(const glm::vec2& pos, const glm::vec2& size, const ref<texture2D>& texture, float textureScale = 1.0f);
		static void drawSquad(const glm::vec3& pos, const glm::vec2& size, const ref<texture2D>& texture, float textureScale = 1.0f);

		static void drawSquad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color, const ref<texture2D>& texture, float textureScale = 1.0f);

		static void drawRotatedSquad(const glm::vec2& pos, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedSquad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void drawRotatedSquad(const glm::vec2& pos, const glm::vec2& size, float rotation, const ref<texture2D>& texture, float textureScale = 1.0f);
		static void drawRotatedSquad(const glm::vec3& pos, const glm::vec2& size, float rotation, const ref<texture2D>& texture, float textureScale = 1.0f);

		static void drawRotatedSquad(const glm::vec3& pos, const glm::vec2& size, float rotation, const glm::vec4& color, const ref<texture2D>& texture, float textureScale = 1.0f);
	};
}

#endif