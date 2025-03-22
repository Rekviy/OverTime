#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "rendererAPI.h"

namespace overtime {
	class renderer {
	public:
		static void beginScene();
		static void endScene();
		static void submit(const std::shared_ptr<vertexArray>& vertexArray);

	private:

	};
}
#endif