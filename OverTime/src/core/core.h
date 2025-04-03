#pragma once
#ifndef CORE_H
#define CORE_H
#include <memory>
	#ifdef OT_DEBUG
		#define OT_ENABLE_ASSERTS
	#endif
	#ifdef OT_ENABLE_ASSERTS
		#include "log.h"
		#define OT_ASSERT(x, ...) { if(!(x)) { OT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define OT_CORE_ASSERT(x, ...) { if(!(x)) { OT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#else
		#define OT_ASSERT(x, ...)
		#define OT_CORE_ASSERT(x, ...)
	#endif

#define BIT(x) (1 << x)

#define OT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace overtime {
	template<typename T>
	using ref = std::shared_ptr<T>;

	template<typename T>
	using scope = std::unique_ptr<T>;
}

#endif