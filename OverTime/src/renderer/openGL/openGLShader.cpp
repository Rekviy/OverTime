#include "openGLShader.h"
#include "core/log.h"
#include "core/core.h"

#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace overtime {
	openGLShader::openGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		:m_Name(name)
	{
		std::unordered_map<type, const std::string> shaderSrcs;
		shaderSrcs.reserve(2);
		shaderSrcs.emplace((type)GL_VERTEX_SHADER, vertexSrc);
		shaderSrcs.emplace((type)GL_FRAGMENT_SHADER, fragmentSrc);
		compileShader(shaderSrcs);
	}

	openGLShader::openGLShader(const std::unordered_map<type, const std::filesystem::path>& shaders)
	{
		std::unordered_map<type, const std::string> shaderSrcs;
		shaderSrcs.reserve(shaders.size());
		for (auto& shader : shaders)
			shaderSrcs.emplace(shader.first, readFile(shader.second));
		compileShader(shaderSrcs);
		m_Name = shaders.begin()->second.stem().string();;
	}

	std::string openGLShader::readFile(const std::filesystem::path& shader)
	{
		std::string res;
		std::ifstream in(shader, std::ios::in | std::ios::binary);
		if (!in)
			OT_CORE_ERROR("Failed to open file '{0}'", shader.string());
		in.seekg(0, std::ios::end);
		res.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&res[0], res.size());
		in.close();

		return res;
	}

	void openGLShader::compileShader(const std::unordered_map<type, const std::string>& shaderSrcs)
	{
		uint32_t program = glCreateProgram();
		std::vector<uint32_t> shaderIds;
		shaderIds.reserve(shaderSrcs.size());

		for (auto& shader : shaderSrcs) {
			uint32_t shaderId = glCreateShader(shader.first);
			const GLchar* source = shader.second.c_str();
			glShaderSource(shaderId, 1, &source, nullptr);

			glCompileShader(shaderId);
			GLint isCompiled = 0;

			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shaderId);

				OT_CORE_CRIT("{0}", infoLog.data());
				OT_CORE_ASSERT(false, "Shader compilation falure!");
				return;
			}
			glAttachShader(program, shaderId);
			shaderIds.push_back(shaderId);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			for (auto id : shaderIds)
				glDeleteShader(id);

			OT_CORE_CRIT("{0}", infoLog.data());
			OT_CORE_ASSERT(false, "Shader linking falure!");
			return;
		}
		for (auto id : shaderIds)
			glDetachShader(program, id);

		m_RendererId = program;
	}
	openGLShader::~openGLShader()
	{
		glDeleteProgram(m_RendererId);
	}

	void openGLShader::bind() const
	{
		glUseProgram(m_RendererId);
	}

	void openGLShader::unbind() const
	{
		glUseProgram(0);
	}

	void openGLShader::uploadUniformInt(const std::string& name, int vector)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniform1i(location, vector);
	}

	void openGLShader::uploadUniformFloat(const std::string& name, float vector)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniform1f(location, vector);
	}

	void openGLShader::uploadUniformFloat2(const std::string& name, const glm::vec2& vector)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniform2f(location, vector.x, vector.y);
	}

	void openGLShader::uploadUniformFloat3(const std::string& name, const glm::vec3& vector)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void openGLShader::uploadUniformFloat4(const std::string& name, const glm::vec4& vector)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	}

	void openGLShader::uploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void openGLShader::uploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererId, name.c_str());
		if (location == -1) {
			OT_CORE_ASSERT(false, "Couldn't find uniform location of {0}", name);
			return;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}
