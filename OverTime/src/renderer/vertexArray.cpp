#include "vertexArray.h"
#include "openGL/openGLVertexArray.h"
#include "rendererAPI.h"

namespace overtime {
	ref<vertexArray> vertexArray::create()
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLVertexArray>();

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}
}