#pragma once
#ifndef LOG_H
#define LOG_H

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace overtime {
	class log {
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& getCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& getClientLogger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_clientLogger;
		static std::shared_ptr<spdlog::logger> s_coreLogger;
	};
}

//CORE LOG MACROS
#define OT_CORE_TRACE(...)  ::overtime::log::getCoreLogger()->trace(__VA_ARGS__)
#define OT_CORE_INFO(...)   ::overtime::log::getCoreLogger()->info(__VA_ARGS__)
#define OT_CORE_WARN(...)   ::overtime::log::getCoreLogger()->warn(__VA_ARGS__)
#define OT_CORE_ERROR(...)  ::overtime::log::getCoreLogger()->error(__VA_ARGS__)
#define OT_CORE_CRIT(...)   ::overtime::log::getCoreLogger()->critical(__VA_ARGS__)

//CLIENT LOG MACROS
#define OT_TRACE(...)  ::overtime::log::getClientLogger()->trace(__VA_ARGS__)
#define OT_INFO(...)   ::overtime::log::getClientLogger()->info(__VA_ARGS__)
#define OT_WARN(...)   ::overtime::log::getClientLogger()->warn(__VA_ARGS__)
#define OT_ERROR(...)  ::overtime::log::getClientLogger()->error(__VA_ARGS__)
#define OT_CRIT(...)   ::overtime::log::getClientLogger()->critical(__VA_ARGS__)
#endif