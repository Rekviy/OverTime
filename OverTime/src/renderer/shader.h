#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>

namespace overtime {
	class shader {
	public:
		shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~shader();
		void bind() const;
		void unbind() const;

		inline const uint32_t& getRendererId() { return m_RendererId; }
	private:
		uint32_t compileShader(uint32_t shaderType, const std::string& src);
		uint32_t m_RendererId;
	};
}
#endif