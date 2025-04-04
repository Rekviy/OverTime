#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>

namespace overtime {
	class shader {
	public:
		virtual ~shader() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static ref<shader> create(const std::string& vertexSrc, const std::string& fragmentSrc);
	};
}
#endif