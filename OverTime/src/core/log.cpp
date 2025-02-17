#include "spdlog/sinks/stdout_color_sinks.h"
#include "log.h"

namespace overtime {
	std::shared_ptr<spdlog::logger> log::s_clientLogger;
	std::shared_ptr<spdlog::logger> log::s_coreLogger;

	void log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_coreLogger = spdlog::stdout_color_mt("OVERTIME");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stdout_color_mt("APP");
		s_clientLogger->set_level(spdlog::level::trace);
	}


}