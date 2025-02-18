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
#endif