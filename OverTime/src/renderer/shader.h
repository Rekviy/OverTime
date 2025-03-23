#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

namespace overtime {
	class shader {
	public:
		shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~shader();
		void bind() const;
		void unbind() const;

		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		inline const uint32_t& getRendererId() { return m_RendererId; }
	private:
		uint32_t compileShader(uint32_t shaderType, const std::string& src);
		uint32_t m_RendererId;
	};
}
#endif