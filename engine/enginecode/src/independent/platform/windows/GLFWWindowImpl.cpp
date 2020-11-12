#include "engine_pch.h"
#include "platform/windows/GLFWWindowImpl.h"
#include "events/keyEvents.h"
#include "events/mouseEvents.h"
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

		m_graphicsContext.reset(new GLFWGraphicsContext(m_Window));
		m_graphicsContext->init();

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_callback);

		glfwSetKeyCallback(m_Window, [](GLFWwindow* win, int keyCode, int scancode, int action, int mods)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			if (action == GLFW_PRESS) {
				KeyPressedEvent keyPressedEvent(keyCode, 0);
				callback(keyPressedEvent);
			}
			else if (action == GLFW_REPEAT) {
				KeyPressedEvent keyRepeatEvent(keyCode, 1);
				callback(keyRepeatEvent);
			}
			else if (action == GLFW_RELEASE) {
				KeyReleasedEvent keyReleasedEvent(keyCode);
				callback(keyReleasedEvent);
			}
		});

		//typed


		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* win, int button, int action, int mods)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			if (action == GLFW_PRESS) {
				MouseButtonPressedEvent mousePressedEvent(button);
				callback(mousePressedEvent);
			}
			else if (action == GLFW_RELEASE) {
				MouseButtonReleasedEvent mouseReleasedEvent(button);
				callback(mouseReleasedEvent);
			}
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* win, double xPos, double yPos)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			MouseMovedEvent mouseMoved(xPos, yPos);
			callback(mouseMoved);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* win, double xOffset, double yOffset)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			MouseScrolledEvent mouseScrolled(xOffset, yOffset);
			callback(mouseScrolled);
		});


		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* win)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			WindowCloseEvent closeEvent;
			callback(closeEvent); 
		});

		glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* win, int entered)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			if (entered == 1) {
				WindowFocusEvent focusEvent;
				callback(focusEvent);
			}
			else if (entered == 0) {
				WindowLostFocusEvent lostFocusEvent;
				callback(lostFocusEvent);
			}
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* win, int xPos, int yPos)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			WindowMovedEvent movedEvent(xPos, yPos);
			callback(movedEvent);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* win, int width, int height)
		{
			std::function<void(Event&)>& callback = *static_cast<std::function<void(Event&)>*>(glfwGetWindowUserPointer(win));

			WindowResizeEvent resizeEvent(width, height);
			callback(resizeEvent);
		});

		setVSync(true);
	}

	void GLFWWindowImpl::close() {
		glfwDestroyWindow(m_Window);
	}

	void GLFWWindowImpl::onUpdate(float timestep) {
		glfwPollEvents();
		m_graphicsContext->swapBuffers();
		//glfwSwapBuffers(m_Window);
	}

	void GLFWWindowImpl::setVSync(bool VSync) {
		if (VSync) glfwSwapInterval(1);
		else glfwSwapInterval(0);

		m_properties.m_isVSync = VSync;
	}

}