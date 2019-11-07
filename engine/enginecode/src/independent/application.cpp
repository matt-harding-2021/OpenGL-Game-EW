/** \file application.cpp
*/


#include "engine_pch.h"
#include "core/application.h"



namespace Engine {

	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		m_Log = std::make_shared<logging>();
		m_Log->start();

		m_Timer = std::make_shared<timer>();
		m_Timer->start();
	}

	Application::~Application()
	{
		m_Timer->stop();
		m_Timer.reset();
		
		m_Log->stop();
		m_Log.reset();
	}

	void Application::onEvent(Event & e)
	{
		switch (e.getEventType())
		{
		case EventType::WindowResize :
			//WindowResize re = 
			break;
		
		}

		if (e.getEventType() == EventType::WindowResize);
	}

	void Application::run()
	{

		bool bRunning = true;
		while(bRunning){
			timer::startFrameTimer();

			

			
			LOG_INFO("fps: {0}", 1.f / timer::getFrameTime());
			float totalTimeElapsed = timer::getMarkerTimer();
			LOG_INFO("totalTimeElapsed: {0}", totalTimeElapsed);
			if (totalTimeElapsed > 3.f) bRunning = false;
		}
		//LOG_INFO("End Time: {0}", totalTimeElapsed);
	}

}
