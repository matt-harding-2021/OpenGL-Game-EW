#include "engine_pch.h"
#include "systems/timer.h"

namespace Engine {
	std::chrono::high_resolution_clock::time_point timer::m_frameStart;
	std::chrono::high_resolution_clock::time_point timer::m_markerStart;
	
	timer::timer()
	{
	}

	timer::~timer()
	{
	}

	void timer::start(SystemSignal init, ...)
	{
		startFrameTimer();
		startMarkerTimer();
	}
	void timer::stop(SystemSignal init, ...)
	{
		
	}

	void timer::startFrameTimer()
	{
		m_frameStart = std::chrono::high_resolution_clock::now();
	}

	float timer::getFrameTime()
	{
		std::chrono::duration<float> timePassed = std::chrono::high_resolution_clock::now() - m_frameStart;
		return timePassed.count();
	}

	void timer::startMarkerTimer()
	{
		m_markerStart = std::chrono::high_resolution_clock::now();
	}

	float timer::getMarkerTimer()
	{
		std::chrono::duration<float> timePassed = std::chrono::high_resolution_clock::now() - m_markerStart;
		return timePassed.count();
	}

	/*float timer::getTimeSeconds()
	{
		
	}

	float timer::getTimeMilliseconds()
	{
		std::chrono::duration<double, std::milli> timePassed = m_end - m_start;
		return timePassed.count();
	}

	float timer::getTimeMicroseconds()
	{
		std::chrono::duration<float, std::micro> timePassed = m_end - m_start;
		return timePassed.count();
	}*/
}