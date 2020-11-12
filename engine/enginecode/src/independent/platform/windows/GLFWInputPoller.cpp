/* \file GLFWInputPoller.cpp */

#include "engine_pch.h"
#include "platform/windows/GLFWInputPoller.h"

#include "systems/logging.h"
namespace Engine {

	GLFWwindow* GLFWInputPoller::s_window = nullptr;

	bool Engine::GLFWInputPoller::isKeyPressed(int keyCode)
	{
		if (s_window) {
			int action = glfwGetKey(s_window, keyCode);
			return action == GLFW_PRESS || action == GLFW_REPEAT;
		}
		return false;
	}

	bool GLFWInputPoller::isMouseButtonPressed(int mouseButton)
	{
		if (s_window) {
			int action = glfwGetMouseButton(s_window, mouseButton);
			return action == GLFW_PRESS;
		}
		return false;
	}

	glm::vec2 GLFWInputPoller::getMousePosition()
	{
		if (s_window) {
			double x, y;
			glfwGetCursorPos(s_window, &x, &y);
			return glm::vec2(static_cast<float>(x), static_cast<float>(y));
		}
		return { 0.0f, 0.0f };
	}
}