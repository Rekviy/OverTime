#include "buffer.h"
#include "openGL/openGLBuffer.h"
#include "renderer.h"
namespace overtime {

	vertexBuffer* vertexBuffer::create(float* vertices, uint32_t size)
	{
		switch (renderer::getAPI()) {
		case rendererAPI::None: return nullptr;
		case rendererAPI::OpenGL: return new openGLVertexBuffer(vertices, size);

		}
	}

	indexBuffer* indexBuffer::create(uint32_t* indices, uint32_t size)
	{
		switch (renderer::getAPI()) {
		case rendererAPI::None: return nullptr;
		case rendererAPI::OpenGL: return new openGLIndexBuffer(indices, size);
		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

}
