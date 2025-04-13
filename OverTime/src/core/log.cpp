#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace overtime {
	ref<spdlog::logger> log::s_clientLogger;
	ref<spdlog::logger> log::s_coreLogger;

	void log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("OVERTIME");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);
	}
}