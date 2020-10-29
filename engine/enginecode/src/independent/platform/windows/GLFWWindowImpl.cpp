#include "engine_pch.h"
#include "platform/windows/GLFWWindowImpl.h"

namespace Engine {



	Window* Window::create(const WindowProperties& properties) { return new GLFWWindowImpl(properties); }

	GLFWWindowImpl::GLFWWindowImpl(const WindowProperties& properties) {
		init(properties);
	}

	GLFWWindowImpl::~GLFWWindowImpl() {
		close();
	}

	void GLFWWindowImpl::init(const WindowProperties& properties) {
		m_Data.Title = properties.m_title;
		m_Data.Height = properties.m_height;
		m_Data.Width = properties.m_width;
		m_Data.VSync = properties.m_isVSync;
		m_Data.Fullscreen = properties.m_isFullScreen;
	


		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
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

		m_Data.VSync = VSync;
	}
}