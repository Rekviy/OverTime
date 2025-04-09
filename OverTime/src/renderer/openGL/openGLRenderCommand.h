#pragma once
#ifndef OPENGLRENDERCOMMAND_H
#define OPENGLRENDERCOMMAND_H

#include "renderer/renderCommand.h"

namespace overtime {
	class openGLRenderCommand : public renderCommand {
	public:
		void init() override;
		void setClearColor(const glm::vec4& color) override;
		void clear() override;
		void drawIndexed(const ref<vertexArray>& vertexArray) override;
		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	private:

	};
}

#endif