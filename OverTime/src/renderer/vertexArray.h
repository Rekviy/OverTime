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

		virtual void addVertexBuffer(const std::shared_ptr<vertexBuffer>& vBuff) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<indexBuffer>& iBuff) = 0;

		virtual const std::vector<std::shared_ptr<vertexBuffer>>& getVertexBuffers() const = 0;
		virtual const std::shared_ptr<indexBuffer>& getIndexBuffer() const = 0;

		static vertexArray* create();
	};
}
#endif