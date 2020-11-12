/* \file GLFWGraphicsContext.cpp */

#include "engine_pch.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "systems/logging.h"
#include "platform/windows/GLFWGraphicsContext.h"

namespace Engine {
	void GLFWGraphicsContext::init()
	{
		glfwMakeContextCurrent(m_window);
		auto result = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
		if (!result) LOG_ERROR("OpenGL loading failed: {0}", result);
	}

	void GLFWGraphicsContext::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}