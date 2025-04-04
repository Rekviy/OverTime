#pragma once
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include "buffer.h"

#include <memory>

namespace overtime {
	class vertexArray {
	public:
		virtual ~vertexArray() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void addVertexBuffer(const ref<vertexBuffer>& vBuff) = 0;
		virtual void setIndexBuffer(const ref<indexBuffer>& iBuff) = 0;

		virtual const std::vector<ref<vertexBuffer>>& getVertexBuffers() const = 0;
		virtual const ref<indexBuffer>& getIndexBuffer() const = 0;

		static ref<vertexArray> create();
	};
}
#endif