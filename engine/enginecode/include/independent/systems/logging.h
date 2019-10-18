/** \file logging.h
*/

#pragma once

#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>

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
		bool isRunning() { return b_active; }
		void log(const std::string& msg);
	private:
		static bool b_active;
		static std::shared_ptr<spdlog::logger> s_logger;
	};

	logging::logging()
	{
						 
	}
	logging::~logging()
	{

	}

	void logging::start(SystemSignal init = SystemSignal::None, ...)
	{
		b_active = true;
	}
	void logging::stop(SystemSignal init = SystemSignal::None, ...)
	{
		s_logger.reset;
		b_active = false;
	}

	void logging::log(const std::string& msg)
	{
		if (b_active) s_logger->log(spdlog::level::info, msg);
	}
	
}
