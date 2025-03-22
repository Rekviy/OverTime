#pragma once
#ifndef RENDERAPI_H
#define RENDERAPI_H


#include "renderCommand.h"

#include <glm/glm.hpp>

namespace overtime {
	class rendererAPI {
	public:
		static inline void setClearColor(const glm::vec4& color) { s_RenderCommands->setClearColor(color); }
		static inline void clear() { s_RenderCommands->clear(); }
		static inline void drawIndexed(const std::shared_ptr<vertexArray>& vertexArray) { s_RenderCommands->drawIndexed(vertexArray); }

		enum class API { None = 0, OpenGL = 1 };
		inline static API getAPI() { return s_RendererAPI; }
	private:
		static API s_RendererAPI;
		static renderCommand* s_RenderCommands;
	};
}

#endif