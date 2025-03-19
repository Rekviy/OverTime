#include "shader.h"
#include "core/log.h"
#include "core/core.h"

#include <vector>
#include <glad/glad.h>

namespace overtime {
	shader::shader(const std::string& vertexSrc, const std::string& fragmentSrc)
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

	shader::~shader()
	{
		glDeleteProgram(m_RendererId);
	}

	void shader::bind() const
	{
		glUseProgram(m_RendererId);
	}

	void shader::unbind() const
	{
		glUseProgram(0);
	}

	uint32_t shader::compileShader(uint32_t shaderType, const std::string& src)
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
}