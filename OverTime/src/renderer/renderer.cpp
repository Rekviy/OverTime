#include "renderer.h"
#include "openGL/openGLShader.h"
namespace overtime {
	renderer::sceneData* renderer::s_SceneData = new renderer::sceneData;

	void renderer::beginScene(orthographCamera& camera)
	{
		s_SceneData->PVMatrix = camera.getPVMatrix();
	}

	void renderer::endScene()
	{}

	void renderer::submit(const ref<vertexArray>& vertexArray, const ref<shader>& shader, const glm::mat4& transform)
	{
		shader->bind();
		std::static_pointer_cast<openGLShader>(shader)->uploadUniformMat4("u_ViewProj", s_SceneData->PVMatrix);
		std::static_pointer_cast<openGLShader>(shader)->uploadUniformMat4("u_Transform", transform);
		vertexArray->bind();
		rendererAPI::drawIndexed(vertexArray);
	}
}