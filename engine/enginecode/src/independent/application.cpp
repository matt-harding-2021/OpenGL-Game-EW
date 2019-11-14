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
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowClose>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResize>(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		/*
		switch (e.getEventType())
		{
		case EventType::WindowResize :
			WindowResize re = (WindowResize&)e;
			LOG_INFO("Window resize event. Width {0}. Height {1}.", re.getWidth(), re.getHeight());
			break;
		
		}

		if (e.getEventType() == EventType::WindowResize);
		*/
	}
	bool Application::onKeyPress(KeyPressed& e)
	{
		return true;
	}
	bool Application::onKeyRelease(KeyReleased& e)
	{
		return true;
	}
	bool Application::onKeyTyped(KeyTyped& e)
	{
		return true;
	}
	bool Application::onMouseButtonPress(MouseButtonPressed& e)
	{
		return true;
	}
	bool Application::onMouseButtonRelease(MouseButtonReleased& e)
	{
		return true;
	}
	bool Application::onMouseMove(MouseMoved& e)
	{
		return true;
	}
	bool Application::onMouseScroll(MouseScrolled& e)
	{
		return true;
	}
	bool Application::onWindowClose(WindowClose & e)
	{
		LOG_INFO("Closing Application");
		bRunning = false;
		return true;
	}
	bool Application::onWindowFocus(WindowFocus& e)
	{
		return true;
	}
	bool Application::onWindowLostFocus(WindowLostFocus& e)
	{
		return true;
	}
	bool Application::onWindowMove(WindowMoved& e)
	{
		return true;
	}
	bool Application::onWindowResize(WindowResize & e)
	{
		LOG_INFO("Resize window to {0}x{1}", e.getWidth(), e.getHeight());
		return true;
	}

	void Application::run()
	{
		while (bRunning) {
			timer::startFrameTimer();




			LOG_INFO("fps: {0}", 1.f / timer::getFrameTime());
			float totalTimeElapsed = timer::getMarkerTimer();
			LOG_INFO("totalTimeElapsed: {0}", totalTimeElapsed);
			if (totalTimeElapsed > 10.f)
			{
				WindowResize e1(1024, 720);
				onEvent(e1);
				WindowClose e2;
				onEvent(e2);
				//LOG_INFO("End Time: {0}", totalTimeElapsed);
			}
		}
	}

}
