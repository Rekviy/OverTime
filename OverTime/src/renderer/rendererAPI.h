#pragma once
#ifndef RENDERAPI_H
#define RENDERAPI_H


#include "renderCommand.h"

#include <glm/glm.hpp>

namespace overtime {
	class rendererAPI {
	public:
		static inline void init() { s_RenderCommands->init(); }
		static inline void setClearColor(const glm::vec4& color) { s_RenderCommands->setClearColor(color); }
		static inline void clear() { s_RenderCommands->clear(); }
		static inline void drawIndexed(const ref<vertexArray>& vertexArray) { s_RenderCommands->drawIndexed(vertexArray); }
		static inline void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RenderCommands->setViewport(x, y, width, height); }

		enum class API { None = 0, OpenGL = 1 };
		inline static API getAPI() { return s_RendererAPI; }
	private:
		static API s_RendererAPI;
		static renderCommand* s_RenderCommands;
	};
}

#endif