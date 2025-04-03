#include "openGLRenderCommand.h"

#include <glad/glad.h>

namespace overtime {
	void openGLRenderCommand::setClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void openGLRenderCommand::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void openGLRenderCommand::drawIndexed(const ref<vertexArray>&vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}