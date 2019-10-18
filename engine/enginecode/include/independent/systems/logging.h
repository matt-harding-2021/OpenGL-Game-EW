/** \file logging.h
*/

#pragma once

#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "system.h"



/**
\class Logging class for writitng to console
*/
namespace Engine {
	class logging : public System
	{
	public:
		logging();
		~logging();
		void start(SystemSignal init = SystemSignal::None, ...);
		void stop(SystemSignal init = SystemSignal::None, ...);
		void log(const std::string& msg);
		void log(const std::string& msg, const float& flt);

		inline static std::shared_ptr<spdlog::logger> getLogger() { return s_logger; }
	private:
		static std::shared_ptr<spdlog::logger> s_logger;
	};
}

#define LOG(...) Engine::logging::getLogger()->log(__VA_ARGS__)
#define LOG_INFO(...) Engine::logging::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Engine::logging::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Engine::logging::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) Engine::logging::getLogger()->critical(__VA_ARGS__)
#define LOG_DEBUG(...) Engine::logging::getLogger()->debug(__VA_ARGS__)
