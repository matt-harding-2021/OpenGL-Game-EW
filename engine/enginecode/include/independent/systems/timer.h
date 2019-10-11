#pragma once

#include <memory>
#include <chrono>


#include "system.h"

namespace Engine {
	class timer : public System
	{
	public:
		timer();
		~timer();

		void start(SystemSignal init = SystemSignal::None, ...);
		void stop(SystemSignal init = SystemSignal::None, ...);
	private:
		std::chrono::high_resolution_clock::time_point m_start;
		std::chrono::high_resolution_clock::time_point m_end;
	};

	timer::timer()
	{
	}

	timer::~timer()
	{
	}

	void timer::start(SystemSignal init = SystemSignal::None, ...)
	{

	}
	void timer::stop(SystemSignal init = SystemSignal::None, ...)
	{

	}

}