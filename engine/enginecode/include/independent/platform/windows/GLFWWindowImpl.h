#pragma once

#include <GLFW/glfw3.h>
#include "systems/logging.h"
#include "windows/window.h"
#include "GLFWGraphicsContext.h"

namespace Engine {
	class GLFWWindowImpl : public Window
	{
	public:
		GLFWWindowImpl(const WindowProperties& properties);
		virtual ~GLFWWindowImpl();

		void onUpdate(float timestep) override;
		void onResize(unsigned int width, unsigned int height) override { return; }
		void setVSync(bool VSync) override;
		void setEventCallback(const std::function<void(Event&)>& callback) override { m_callback = callback; };

		inline unsigned int getWidth() const override { return m_properties.m_width; }
		inline unsigned int getHeight() const override { return m_properties.m_height; }
		
		inline void* getNativeWindow() const override { return m_Window; }
		
		inline bool isFullScreenMode() const override { return m_properties.m_isFullScreen; }
		
		inline bool isVSync() const override { return m_properties.m_isVSync; }

	private:
		GLFWwindow* m_Window;

		void init(const WindowProperties& properties);
		virtual void close() override;
		WindowProperties m_properties;
		std::function<void(Event&)> m_callback;
		float m_aspectRatio;
	};
}
