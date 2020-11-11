/** \file application.cpp */
#include "engine_pch.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/windows/GLFWWindowsSystem.h"
#include "platform/windows/GLFWWindowImpl.h"
#endif
//next, link events to glfw window

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


#ifdef NG_PLATFORM_WINDOWS
		m_windowsSystem = std::make_shared<GLFWWindowsSystem>();
		m_windowsSystem->start();

		WindowProperties winProps;
		m_Window = std::make_unique<GLFWWindowImpl>(winProps);

		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
#endif
	}

	Application::~Application()
	{
		m_windowsSystem->stop();
		m_windowsSystem.reset();

		m_Timer->stop();
		m_Timer.reset();
		
		m_Log->stop();
		m_Log.reset();
	}

	void Application::onEvent(Event & e)
	{
		//LOG_INFO(e);
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		dispatcher.dispatch<KeyReleasedEvent>(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));
		//typed

		dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&Application::onMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.dispatch<MouseButtonReleasedEvent>(std::bind(&Application::onMouseButtonReleased, this, std::placeholders::_1));
		dispatcher.dispatch<MouseMovedEvent>(std::bind(&Application::onMouseMoved, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrolledEvent>(std::bind(&Application::onMouseScrolled, this, std::placeholders::_1));

		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowFocusEvent>(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowLostFocusEvent>(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowMovedEvent>(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
	}
	
	bool Application::onKeyPressed(KeyPressedEvent& e) { 
		if (e.GetRepeatCount() == 0) LOG_INFO("Key Pressed: {0}", e.GetKeyCode());
		else if (e.GetRepeatCount() == 1) LOG_INFO("Key Repeated: {0}", e.GetKeyCode());
		return true; 
	}
	bool Application::onKeyReleased(KeyReleasedEvent& e) { 
		LOG_INFO("Key Released: {0}", e.GetKeyCode());
		return true; 
	}
	bool Application::onKeyTyped(KeyTypedEvent& e) { return true; } 
	bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) { 
		LOG_INFO("Mouse Button Pressed: {0}", e.getButton());
		return true;
	}
	bool Application::onMouseButtonReleased(MouseButtonReleasedEvent& e) {
		LOG_INFO("Mouse Button Released: {0}", e.getButton());
		return true;
	}
	bool Application::onMouseMoved(MouseMovedEvent& e) { 
		LOG_INFO("Mouse Moved: {0}x{1}", e.getPos().x, e.getPos().y);
		return true; 
	}
	bool Application::onMouseScrolled(MouseScrolledEvent& e) { 
		LOG_INFO("Mouse Scrolled: {0}x{1}", e.getXOffset(), e.getYOffset());
		return true; 
	}
	bool Application::onWindowClose(WindowCloseEvent& e) {
		LOG_INFO("Window Closed");
		bRunning = false;
		return true;
	}
	bool Application::onWindowFocus(WindowFocusEvent& e) { 
		LOG_INFO("Window Focused");
		return true; 
	}
	bool Application::onWindowLostFocus(WindowLostFocusEvent& e) { 
		LOG_INFO("Window Lost Focus");
		return true; 
	}
	bool Application::onWindowMoved(WindowMovedEvent& e) { 
		LOG_INFO("Window Moved: {0}x{1}", e.getPos().x, e.getPos().y);
		return true; 
	}
	bool Application::onWindowResize(WindowResizeEvent& e) {
		LOG_INFO("Window Resized: {0}x{1}", e.getWidth(), e.getHeight());
		return true;
	}

	void Application::run()
	{

		while (bRunning) {
			timer::startFrameTimer();


			//LOG_INFO("fps: {0}", 1.f / timer::getFrameTime());
			float totalTimeElapsed = timer::getMarkerTimer();
			//LOG_INFO("totalTimeElapsed: {0}", totalTimeElapsed);
			
			/*if (totalTimeElapsed > 3.f)
			{
				/*
				KeyPressedEvent e1;//Keycode and repeatcount
				KeyReleasedEvent e2;//Keycode
				KeyTypedEvent e3;//Keycode
				MouseButtonPressedEvent e4;
				MouseButtonPressedEvent e5;
				MouseMovedEvent e6;
				MouseScrolledEvent e7;
				WindowFocusEvent e8;
				WindowLostFocusEvent e9;
				WindowMovedEvent e10);
				
				WindowResizeEvent e1(1024, 720);
				onEvent(e1);
				WindowCloseEvent e2;
				onEvent(e2);
				//LOG_INFO("End Time: {0}", totalTimeElapsed);
			}*/

			m_Window->onUpdate(timer::getFrameTime());
		}
	}

}
