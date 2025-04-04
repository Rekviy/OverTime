#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/core.h"

namespace overtime {
	class texture {
	public:
		virtual ~texture() = default;
		virtual int32_t getWidth() const = 0;
		virtual int32_t getHeight() const = 0;
		virtual void bind(uint32_t slot = 0) const = 0;
	};

	class texture2D : public texture {
	public:
		static ref<texture2D> create(const std::string& path);
	};

}


#endif