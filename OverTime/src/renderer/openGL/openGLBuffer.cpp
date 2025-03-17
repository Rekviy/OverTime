#include <glad/glad.h>

#include "openGLBuffer.h"

namespace overtime {

	//VERTEX BUFFER

	openGLVertexBuffer::openGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}
	openGLVertexBuffer::~openGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}
	void openGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	}
	void openGLVertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	//INDEX BUFFER

	openGLIndexBuffer::openGLIndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		glCreateBuffers(1, &m_RendererId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}
	openGLIndexBuffer::~openGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererId);
	}
	void openGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	
	}
	void openGLIndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

