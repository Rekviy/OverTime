#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "vertexArray.h"

#include <glm/glm.hpp>
namespace overtime {
	class renderCommand {
	public:
		virtual void init() = 0;
		virtual void setClearColor(const glm::vec4& color) = 0;
		virtual void clear() = 0;
		virtual void drawIndexed(const ref<vertexArray>& vertexArray) = 0;
	private:
	 
	};
}

#endif