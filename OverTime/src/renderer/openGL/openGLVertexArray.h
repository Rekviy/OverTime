#pragma once
#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include "renderer/vertexArray.h"

#include <memory>
#include <vector>
namespace overtime {
	class openGLVertexArray : public vertexArray {
	public:
		openGLVertexArray();
		virtual ~openGLVertexArray() override;
		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void addVertexBuffer(const ref<vertexBuffer>& vBuff) override;
		virtual void setIndexBuffer(const ref<indexBuffer>& iBuff) override;

		virtual const std::vector<ref<vertexBuffer>>& getVertexBuffers() const { return m_VertexBuffers; };
		virtual const ref<indexBuffer>& getIndexBuffer() const { return m_IndexBuffer; } ;

	private:
		uint32_t m_RendererId;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<ref<vertexBuffer>> m_VertexBuffers;
		ref<indexBuffer> m_IndexBuffer;
	};
}
#endif