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
	private:

	};
}

#endif