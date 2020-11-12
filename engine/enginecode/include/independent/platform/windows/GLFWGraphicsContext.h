/* \GLFWGraphicsContext */
#pragma once

#include "windows/graphicsContext.h"

namespace Engine {
	class GLFWGraphicsContext : public GraphicsContext
	{
	public:
		GLFWGraphicsContext(GLFWwindow* win) : m_window(win) {}
		virtual void init() override;
		virtual void swapBuffers() override;
	private:
		GLFWwindow* m_window;
	};
}