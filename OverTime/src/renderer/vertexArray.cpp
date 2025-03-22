#include "vertexArray.h"
#include "openGL/openGLVertexArray.h"
#include "rendererAPI.h"

namespace overtime {
	vertexArray* vertexArray::create()
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return new openGLVertexArray();

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}
}