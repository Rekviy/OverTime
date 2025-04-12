#include "renderer.h"
#include "renderer2D.h"
#include "openGL/openGLShader.h"

namespace overtime {
	renderer::sceneData* renderer::s_SceneData = new renderer::sceneData;

	void renderer::init()
	{
		rendererAPI::init();
		renderer2D::init();
	}

	void renderer::beginScene(orthographCamera& camera)
	{
		s_SceneData->PVMatrix = camera.getPVMatrix();
	}

	void renderer::endScene()
	{}

	void renderer::submit(const ref<vertexArray>& vertexArray, const ref<shader>& shader, const glm::mat4& transform)
	{
		shader->bind();
		shader->setMat4("u_ProjView", s_SceneData->PVMatrix);
		shader->setMat4("u_Transform", transform);
		vertexArray->bind();
		rendererAPI::drawIndexed(vertexArray);
	}
	void renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		rendererAPI::setViewport(0, 0, width, height);
	}
}