#include "openGLShader.h"
#include "core/log.h"
#include "core/core.h"

#include <vector>
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace overtime {
	openGLShader::openGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
		uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
		m_RendererId = linkShader(vs, fs);
	}
	openGLShader::openGLShader(const std::filesystem::path& vertex, const std::filesystem::path& fragment)
	{
		uint32_t vs = compileShader(GL_VERTEX_SHADER, readFile(vertex));
		uint32_t fs = compileShader(GL_FRAGMENT_SHADER, readFile(fragment));
		m_RendererId = linkShader(vs, fs);
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
	uint32_t openGLShader::compileShader(uint32_t shaderType, const std::string& src)
	{
		uint32_t id = glCreateShader(shaderType);

		const GLchar* source = src.c_str();
		glShaderSource(id, 1, &source, nullptr);

		glCompileShader(id);
		GLint isCompiled = 0;

		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(id);

			OT_CORE_CRIT("{0}", infoLog.data());
			OT_CORE_ASSERT(false, "Shader compilation falure!");
			return -1;
		}
		return id;
	}
	uint32_t openGLShader::linkShader(uint32_t vertexSource, uint32_t fragmentSource)
	{
		uint32_t id = glCreateProgram();

		if (vertexSource == UINT32_MAX || fragmentSource == UINT32_MAX)
			return -1;


		glAttachShader(id, vertexSource);
		glAttachShader(id, fragmentSource);

		glLinkProgram(id);

		GLint isLinked = 0;
		glGetProgramiv(id, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(id);

			glDeleteShader(vertexSource);
			glDeleteShader(fragmentSource);

			OT_CORE_CRIT("{0}", infoLog.data());
			OT_CORE_ASSERT(false, "Shader linking falure!");
			return -1;
		}

		glDetachShader(id, vertexSource);
		glDetachShader(id, fragmentSource);
		return id;
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

	void openGLShader::uploadUniformFloat2(const std::string & name, const glm::vec2 & vector)
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
