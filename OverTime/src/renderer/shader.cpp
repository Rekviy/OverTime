#include "shader.h"
#include "openGL/openGLShader.h"
#include "core/log.h"
#include "core/core.h"


namespace overtime {
	shader* shader::create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return new openGLShader(vertexSrc, fragmentSrc);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

}