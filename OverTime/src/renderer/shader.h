#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <filesystem>
#include <unordered_map>

//TEMP!!!
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31

namespace overtime {
	class shader {
	public:
		enum type {
			vertex = GL_VERTEX_SHADER, fragment = GL_FRAGMENT_SHADER, pixel = fragment
		};
		virtual ~shader() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual const std::string& getName() const = 0;

		static ref<shader> create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		static ref<shader> create(const std::unordered_map<type, const std::filesystem::path>& shaders);
	};

	class shaderLibrary {
	public:
		void add(const std::string& name,const ref<shader>& shader);
		void add(const ref<shader>& shader);
		ref<shader> load(const std::unordered_map<shader::type, const std::filesystem::path>& shaders);
		ref<shader> load(const std::string& name, const std::unordered_map<shader::type, const std::filesystem::path>& shaders);
		ref<shader> get(const std::string& name);
		bool exist(const std::string& name) const;
	private:
		std::unordered_map<std::string, ref<shader>> m_Shaders;
	};
}
#endif