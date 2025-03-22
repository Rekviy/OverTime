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

		virtual void addVertexBuffer(const std::shared_ptr<vertexBuffer>& vBuff) override;
		virtual void setIndexBuffer(const std::shared_ptr<indexBuffer>& iBuff) override;

		virtual const std::vector<std::shared_ptr<vertexBuffer>>& getVertexBuffers() const { return m_VertexBuffers; };
		virtual const std::shared_ptr<indexBuffer>& getIndexBuffer() const { return m_IndexBuffer; } ;

	private:
		uint32_t m_RendererId;
		std::vector<std::shared_ptr<vertexBuffer>> m_VertexBuffers;
		std::shared_ptr<indexBuffer> m_IndexBuffer;
	};
}
#endif