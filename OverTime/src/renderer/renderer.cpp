#include "renderer.h"

namespace overtime {
	void renderer::beginScene()
	{}

	void renderer::endScene()
	{}

	void renderer::submit(const std::shared_ptr<vertexArray>&vertexArray)
	{
		vertexArray->bind();
		rendererAPI::drawIndexed(vertexArray);
	}
}