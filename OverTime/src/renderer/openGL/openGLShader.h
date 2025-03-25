#pragma once
#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "renderer/shader.h"

#include <string>
#include <glm/glm.hpp>
namespace overtime {
	class openGLShader : public shader{
	public:
		openGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~openGLShader() override;
		virtual void bind() const override;
		virtual void unbind() const override;

		void uploadUniformInt(const std::string& name, int vector);

		void uploadUniformFloat(const std::string& name, float vector);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& vector);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& vector);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& vector);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		inline const uint32_t& getRendererId() { return m_RendererId; }
	private:
		uint32_t compileShader(uint32_t shaderType, const std::string& src);
		uint32_t m_RendererId;
	};
}


#endif
