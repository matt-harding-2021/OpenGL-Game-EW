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

		static void startFrameTimer();
		static float getFrameTime();

		static void startMarkerTimer();
		static float getMarkerTimer();

		//float getTimeSeconds();
		//float getTimeMilliseconds();
		//float getTimeMicroseconds();
	private:
		static std::chrono::high_resolution_clock::time_point m_frameStart;
		static std::chrono::high_resolution_clock::time_point m_markerStart;

		/*std::chrono::high_resolution_clock::duration getTime() { return m_end - m_start; }
		std::chrono::high_resolution_clock::time_point m_start;
		std::chrono::high_resolution_clock::time_point m_end;*/
	};
}