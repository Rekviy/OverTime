#pragma once
#ifndef OPENGLRENDERCOMMAND_H
#define OPENGLRENDERCOMMAND_H

#include "renderer/renderCommand.h"

namespace overtime {
	class openGLRenderCommand : public renderCommand {
	public:
		void setClearColor(const glm::vec4& color) override;
		void clear() override;
		void drawIndexed(const std::shared_ptr<vertexArray>& vertexArray) override;
	private:

	};
}

#endif