#pragma once
#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "renderer/shader.h"

#include <string>
#include <glm/glm.hpp>
namespace overtime {
	class openGLShader : public shader{
	public:
		openGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		openGLShader(const std::unordered_map<type, const std::filesystem::path>& shaders);

		virtual ~openGLShader() override;
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual const std::string& getName() const override { return m_Name; };

		virtual void setInt(const std::string& name, int vector) override;

		virtual void setFloat(const std::string& name, float vector) override;
		virtual void setFloat2(const std::string& name, const glm::vec2& vector) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& vector) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& vector) override;

		virtual void setMat3(const std::string& name, const glm::mat3& matrix) override;
		virtual void setMat4(const std::string& name, const glm::mat4& matrix) override;

		void uploadUniformInt(const std::string& name, int vector);

		void uploadUniformFloat(const std::string& name, float vector);
		void uploadUniformFloat2(const std::string& name, const glm::vec2& vector);
		void uploadUniformFloat3(const std::string& name, const glm::vec3& vector);
		void uploadUniformFloat4(const std::string& name, const glm::vec4& vector);

		void uploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void uploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		inline const uint32_t& getRendererId() { return m_RendererId; }
	private:
		std::string readFile(const std::filesystem::path&);
		void compileShader(const std::unordered_map<type, const std::string>& shaderSrcs);
		std::string m_Name;
		uint32_t m_RendererId;
	};
}


#endif
