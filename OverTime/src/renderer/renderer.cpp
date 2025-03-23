#include "renderer.h"

namespace overtime {
	renderer::sceneData* renderer::s_SceneData = new renderer::sceneData;

	void renderer::beginScene(orthographCamera& camera)
	{
		s_SceneData->PVMatrix = camera.getPVMatrix();
	}

	void renderer::endScene()
	{}

	void renderer::submit(const std::shared_ptr<vertexArray>& vertexArray, const std::shared_ptr<shader>& shader)
	{
		shader->bind();
		shader->uploadUniformMat4("u_ViewProj", s_SceneData->PVMatrix);
		vertexArray->bind();
		rendererAPI::drawIndexed(vertexArray);
	}
}