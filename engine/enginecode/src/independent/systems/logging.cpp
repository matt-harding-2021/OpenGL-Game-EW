#include "engine_pch.h"
#include "systems/logging.h"

namespace Engine {
	std::shared_ptr<spdlog::logger> logging::s_logger;
	
	logging::logging()
	{

	}
	logging::~logging()
	{

	}

	void logging::start(SystemSignal init, ...)
	{
		s_logger = spdlog::stdout_color_mt("console");
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_logger->info("Logger Started.");
	}
	void logging::stop(SystemSignal init, ...)
	{
		s_logger->info("Logger Stopped.");
		s_logger.reset();
	}

	void logging::log(const std::string& msg)
	{
		s_logger->log(spdlog::level::info, msg);
	}
	void logging::log(const std::string& msg, const float& flt)
	{
		s_logger->log(spdlog::level::info, msg);
		s_logger->log(spdlog::level::info, flt);
	}

}