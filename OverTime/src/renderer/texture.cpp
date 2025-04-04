#include "texture.h"
#include "openGL/openGLTexture.h"
#include "rendererAPI.h"

namespace overtime {
	ref<texture2D> texture2D::create(const std::string& path)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLTexture2D>(path);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

}
