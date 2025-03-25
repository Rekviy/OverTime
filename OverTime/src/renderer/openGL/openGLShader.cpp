#include "openGLShader.h"
#include "core/log.h"
#include "core/core.h"

#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace overtime {
	openGLShader::openGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		uint32_t vs = compileShader(GL_VERTEX_SHADER, vertexSrc);
		uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
		if (vs == UINT32_MAX || fs == UINT32_MAX)
			return;

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererId = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererId, vs);
		glAttachShader(m_RendererId, fs);

		// Link our program
		glLinkProgram(m_RendererId);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererId, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererId, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererId);
			// Don't leak shaders either.
			glDeleteShader(vs);
			glDeleteShader(fs);

			OT_CORE_CRIT("{0}", infoLog.data());
			OT_CORE_ASSERT(false, "Shader linking falure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererId, vs);
		glDetachShader(m_RendererId, fs);
	}

	uint32_t openGLShader::compileShader(uint32_t shaderType, const std::string& src)
	{
		// Create an empty vertex shader handle
		uint32_t id = glCreateShader(shaderType);

		// Send the shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = src.c_str();
		glShaderSource(id, 1, &source, nullptr);

		// Compile the shader
		glCompileShader(id);
		GLint isCompiled = 0;

		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(id);

			OT_CORE_CRIT("{0}", infoLog.data());
			OT_CORE_ASSERT(false, "Shader compilation falure!");
			return -1;
		}
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
