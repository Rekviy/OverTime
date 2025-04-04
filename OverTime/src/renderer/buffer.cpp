#include "buffer.h"
#include "openGL/openGLBuffer.h"
#include "rendererAPI.h"
#include "core/core.h"

namespace overtime {

	ref<vertexBuffer> vertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLVertexBuffer>(vertices, size);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

	ref<indexBuffer> indexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLIndexBuffer>(indices, size);
		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

}
