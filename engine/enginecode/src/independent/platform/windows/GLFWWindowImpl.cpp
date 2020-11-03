#include "engine_pch.h"
#include "platform/windows/GLFWWindowImpl.h"
#include "events/windowEvents.h"

namespace Engine {



	Window* Window::create(const WindowProperties& properties) { return new GLFWWindowImpl(properties); }

	GLFWWindowImpl::GLFWWindowImpl(const WindowProperties& properties) {
		init(properties);
	}

	GLFWWindowImpl::~GLFWWindowImpl() {
		close();
	}

	void GLFWWindowImpl::init(const WindowProperties& properties) {
		m_properties.m_title = properties.m_title;
		m_properties.m_height = properties.m_height;
		m_properties.m_width = properties.m_width;
		m_properties.m_isVSync = properties.m_isVSync;
		m_properties.m_isFullScreen = properties.m_isFullScreen;
	


		m_Window = glfwCreateWindow(m_properties.m_width, m_properties.m_height, m_properties.m_title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_callback);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* win)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			WindowCloseEvent closeEvent;
			callback(closeEvent);
		});

		/*
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* win)
		{

		});
		*/
		setVSync(true);
	}

	void GLFWWindowImpl::close() {
		glfwDestroyWindow(m_Window);
	}

	void GLFWWindowImpl::onUpdate(float timestep) {
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void GLFWWindowImpl::setVSync(bool VSync) {
		if (VSync) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_properties.m_isVSync = VSync;
	}

}