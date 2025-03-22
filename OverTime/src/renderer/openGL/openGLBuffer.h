#pragma once
#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H
#include "renderer/buffer.h"

namespace overtime {
	class openGLVertexBuffer : public vertexBuffer {
	public:
		openGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~openGLVertexBuffer() override;

		virtual void unbind() const override;
		virtual void bind() const override;

		virtual const bufferLayout& getLayout() const override { return m_Layout; };
		virtual void setLayout(const bufferLayout& layout) override { m_Layout = layout; };
	private:
		uint32_t m_RendererId;
		bufferLayout m_Layout;
	};
	class openGLIndexBuffer : public indexBuffer {
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