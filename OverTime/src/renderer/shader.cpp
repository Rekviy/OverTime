#include "shader.h"
#include "openGL/openGLShader.h"
#include "core/log.h"
#include "core/core.h"
#include "rendererAPI.h"


namespace overtime {
	ref<shader> shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLShader>(name, vertexSrc, fragmentSrc);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}
	ref<shader> shader::create(const std::unordered_map<type, const std::filesystem::path>& shaders)
	{
		switch (rendererAPI::getAPI()) {
			case rendererAPI::API::None: return nullptr;
			case rendererAPI::API::OpenGL: return std::make_shared<openGLShader>(shaders);

		}
		OT_CORE_ASSERT(false, "Unknown renderAPI!");
		return nullptr;
	}

	void shaderLibrary::add(const std::string& name, const ref<shader>& shader)
	{
		OT_CORE_ASSERT(!exist(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void shaderLibrary::add(const ref<shader>& shader)
	{
		auto& name = shader->getName();
		add(name, shader);
	}
	ref<shader> shaderLibrary::load(const std::unordered_map<shader::type, const std::filesystem::path>& shaders)
	{
		auto shader = shader::create(shaders);
		add(shader);
		return shader;
	}
	ref<shader> shaderLibrary::load(const std::string& name, const std::unordered_map<shader::type, const std::filesystem::path>& shaders)
	{
		auto shader = shader::create(shaders);
		add(name,shader);
		return shader;
	}
	ref<shader> shaderLibrary::get(const std::string& name)
	{
		OT_CORE_ASSERT(exist(name), "Shader doesn't exists!");
		return m_Shaders[name];
	}
	bool shaderLibrary::exist(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}