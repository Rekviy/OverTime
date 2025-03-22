#include "buffer.h"
#include "openGL/openGLBuffer.h"
#include "rendererAPI.h"
#include "core/core.h"

namespace overtime {

	vertexBuffer* vertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return new openGLVertexBuffer(vertices, size);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

	indexBuffer* indexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return new openGLIndexBuffer(indices, size);
		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

}
