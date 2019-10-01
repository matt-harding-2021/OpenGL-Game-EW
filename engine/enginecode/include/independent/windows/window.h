#pragma once

#include "graphicsContext.h"
#include <string>
#include <functional>

namespace Engine {

	class Event; // Be replaced

	struct WindowProperties
	{
		std::string m_title;
		unsigned int m_width;
		unsigned int m_height;
		bool m_isFullScreen;
		bool m_isVSync;

		WindowProperties(const std::string& title = "My Window", unsigned int width = 800, unsigned int height = 600, bool fullscreen = false) : m_title(title), m_width(width), m_height(height), m_isFullScreen(fullscreen) {}
	};

	class Window
	{
	public:
		virtual void init(const WindowProperties& properties) = 0;
		virtual void close() = 0;
		virtual ~Window() {};
		virtual void onUpdate(float timestep) = 0;
		virtual void onResize(unsigned int width, unsigned int height) = 0;
		virtual void setVSync(bool VSync) = 0;
		virtual void setEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;
		virtual void* getNativeWindow() const = 0;
		virtual bool isFullScreenMode() const = 0;
		virtual bool isVSync() const = 0;

		static Window* create(const WindowProperties& properties = WindowProperties());
	protected:
		GraphicsContext* m_context;
	};
}

