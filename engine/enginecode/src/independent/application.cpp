/** \file application.cpp
*/


#include "engine_pch.h"

// temp includes
#include <glad/glad.h>
#include <gl/GL.h>

#include <stb_image.h>


#include "core/application.h"
#ifdef NG_PLATFORM_WINDOWS
#include "../platform/windows/GLFWWindowsSystem.h"
#endif // NG_PLATFORM_WINDOWS



namespace Engine {
	Application* Application::s_instance = nullptr;
	glm::ivec2 Application::s_screenResolution = glm::ivec2(0, 0);
	std::shared_ptr<ResourceManager> Application::s_resources = nullptr;
	float Application::s_timestep = 0.f;

	Application::Application()
	{
		if (s_instance == nullptr)
		{
			s_instance = this;
		}


		// Allocate and start systems
		m_log = std::make_shared<Log>(Log());
		m_log->start();
		NG_ASSERT(Engine::Log::isRunning(), "Log not started")

		s_resources = std::make_shared<ResourceManager>(ResourceManager());

		m_timer = std::make_shared<Timer>(Timer());
		ENGINE_INFO("Timer system started");

#ifdef NG_PLATFORM_WINDOWS
		m_windows = std::shared_ptr<Windows>(new GLFWWindowsSystem());	
#endif // NG_PLATFORM_WINDOWS
		m_windows->start();
		ENGINE_INFO("Windows system initialised");

		m_window = std::shared_ptr<IWindow>(IWindow::create());
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
		// Set screen res
		Application::s_screenResolution = glm::ivec2(m_window->getWidth(), m_window->getHeight());

		m_layerStack = std::make_shared<LayerStack>(LayerStack());
		m_layerStack->start();
		ENGINE_INFO("Layer stack initialised");
	
		float f = m_timer->getFrameTimeSeconds(); // make sure timer has been reset after starting systems and allocating resources
	}

	Application::~Application()
	{
		// Stop systems
		m_layerStack->stop();
		ENGINE_INFO("Layer stack stopped");

		m_windows->stop();
		ENGINE_INFO("Windows system stopped");

		m_timer->stop();
		ENGINE_INFO("Timing system stopped");

		s_resources->stop();
		ENGINE_INFO("Resource manager stopped");

		ENGINE_INFO("Stopping Logging system");
		m_log->stop();
		NG_ASSERT(!Engine::Log::isRunning(), "Log not stopped")
	}

	void Application::onEvent(Event & e)
	{
		//ENGINE_INFO("{0}", e);
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onResize, this, std::placeholders::_1));
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));

		for (auto it = m_layerStack->end(); it != m_layerStack->begin(); )
		{
			(*--it)->onEvent(e);
			if (e.handled()) break;
		}
	}

	void Application::pushLayer(const std::shared_ptr<Layer>& layer)
	{
		m_layerStack->push(layer);
	}

	void Application::popLayer()
	{
	}

	bool Application::onClose(WindowCloseEvent& e)
	{
		m_running = false;
		m_window->close();
		return true;
	}

	bool Application::onResize(WindowResizeEvent & e)
	{
		m_window->correctViewport(e.getWidth(), e.getHeight());
		return true;
	}

	bool Application::onKeyPressed(KeyPressedEvent & e)
	{
		if (e.GetKeyCode() == 256) //Escape
		{
			m_running = false;
			m_window->close();
			return true;
		}
		return false;
	}

	void Application::run()
	{
		while (m_running)
		{
			// Run game loop
			s_timestep = m_timer->getFrameTimeSeconds();
			ENGINE_INFO("FPS: {0}", (int)(1.0f / s_timestep));

			for (auto it = m_layerStack->begin(); it != m_layerStack->end(); it++)
				(*it)->onUpdate(s_timestep);

			m_window->onUpdate(s_timestep);
		}
	}

}
