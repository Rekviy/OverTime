#include "openGLVertexArray.h"
#include "core/core.h"

#include <glad/glad.h>
namespace overtime {
	openGLVertexArray::openGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
	}
	openGLVertexArray::~openGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}
	void openGLVertexArray::bind() const
	{
		glBindVertexArray(m_RendererId);
	}
	void openGLVertexArray::unbind() const
	{
		glBindVertexArray(0);

	}
	void openGLVertexArray::addVertexBuffer(const std::shared_ptr<vertexBuffer>& vBuff)
	{
		OT_CORE_ASSERT(vBuff->getLayout().getElements().size(), "Vertex buffer has no layout!");
		glBindVertexArray(m_RendererId);
		vBuff->bind();
		uint32_t index = 0;
		auto shaderDataTypeToOpenGLType = [](shaderDataType type) ->GLenum {
			switch (type) {
				case shaderDataType::Bool:   return GL_BOOL;
				case shaderDataType::Int:
				case shaderDataType::Int2:
				case shaderDataType::Int3:
				case shaderDataType::Int4:   return GL_INT;
				case shaderDataType::Float:
				case shaderDataType::Float2:
				case shaderDataType::Float3:
				case shaderDataType::Float4:
				case shaderDataType::Mat3:
				case shaderDataType::Mat4:   return GL_FLOAT;
			}
			OT_CORE_ASSERT(false, "Unknown shaderDataType!");
			return 0;
		};
		auto& layout = vBuff->getLayout();
		for (const auto& elem : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, elem.getComponentCount(), shaderDataTypeToOpenGLType(elem.type),
				elem.normalized ? GL_TRUE : GL_FALSE, layout.getStride(), (const void*)elem.offset);
			index++;
		}
		m_VertexBuffers.push_back(vBuff);
	}
	void openGLVertexArray::setIndexBuffer(const std::shared_ptr<indexBuffer>& iBuff)
	{
		glBindVertexArray(m_RendererId);
		iBuff->bind();
		m_IndexBuffer = iBuff;
	}

}