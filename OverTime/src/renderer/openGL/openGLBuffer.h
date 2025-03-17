#pragma once
#ifndef OPENGLBUFFER_H
#include "renderer/buffer.h"

namespace overtime {
	class openGLVertexBuffer :public vertexBuffer {
	public:
		openGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~openGLVertexBuffer() override;

		virtual void bind() const override;
		virtual void unbind() const override;
	private:
		uint32_t m_RendererId;
	};
	class openGLIndexBuffer :public indexBuffer {
	public:
		openGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~openGLIndexBuffer() override;
		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererId;
		uint32_t m_Count;
	};
}
#endif