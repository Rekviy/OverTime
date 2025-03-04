#pragma once
#ifndef CORE_H
#define CORE_H
	#ifdef OT_PLATFORM_WINDOWS
		#ifdef OT_BUILD_DLL
			#define OVERTIME_API __declspec(dllexport)
		#else
			#define OVERTIME_API __declspec(dllimport)
		#endif


	#else
		#error ONLY SUPPORT WINDOWS!!
	#endif

	#define BIT(x) (1 << x)
	#ifdef OT_DEBUG
		#define OT_ENABLE_ASSERTS
	#endif
	#ifdef OT_ENABLE_ASSERTS
		#define OT_ASSERT(x, ...) { if(!(x)) { OT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
		#define OT_CORE_ASSERT(x, ...) { if(!(x)) { OT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#else
		#define OT_ASSERT(x, ...)
		#define OT_CORE_ASSERT(x, ...)
	#endif
#define OT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
#endif