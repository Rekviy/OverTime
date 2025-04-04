#include "shader.h"
#include "openGL/openGLShader.h"
#include "core/log.h"
#include "core/core.h"
#include "rendererAPI.h"


namespace overtime {
	ref<shader> shader::create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLShader>(vertexSrc, fragmentSrc);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}
	ref<shader> shader::create(const std::filesystem::path& vertex, const std::filesystem::path& fragment)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLShader>(vertex, fragment);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

}