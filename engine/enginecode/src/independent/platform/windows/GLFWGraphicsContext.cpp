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
		
		//OpenGL Error Log
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](
			GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar *message,
			const void *userParam
			)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				LOG_CRITICAL(message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				LOG_ERROR(message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				LOG_WARN(message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				LOG_INFO(message);
				break;
			}
		},nullptr);
	}

	void GLFWGraphicsContext::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}
}