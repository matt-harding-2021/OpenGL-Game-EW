#pragma once

#include "systems/logging.h"
#include "windows/window.h"
#include <GLFW/glfw3.h>

namespace Engine {
	class GLFWWindowImpl : public Window
	{
	public:
		GLFWWindowImpl(const WindowProperties& properties);
		virtual ~GLFWWindowImpl();

		void onUpdate(float timestep) override;



		inline unsigned int getWidth() const override { return m_Data.Width; }
		inline unsigned int getHeight() const override { return m_Data.Height; }
		bool isFullScreenMode() const override { return m_Data.Fullscreen; }
		bool isVSync() const override { return m_Data.VSync; }
		virtual void* getNativeWindow() const override { return m_Window; }

		void setVSync(bool VSync) override;
		inline void setEventCallback(const std::function<void(Event&)>& callback) override { m_Data.EventCallback = callback; }



		void init(const WindowProperties& properties);
		virtual void close() override;


		void onResize(unsigned int width, unsigned int height) override { return; }

		

	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			bool Fullscreen;

			std::function<void(Event&)>& EventCallback = std::function<void(Event&)>();
		};

		WindowData m_Data;
	};
}
