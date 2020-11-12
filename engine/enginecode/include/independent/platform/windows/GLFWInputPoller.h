/* \file GLFWInputPoller.h */

#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Engine {

	/* \class GLFWInputPoller
	*	Input poller for getting current keyboard/mouse state, specific to GLFw system.
	*/
	class GLFWInputPoller
	{
	public:
		static bool isKeyPressed(int keyCode); //!< Returns a bool based on whether the specific key code is currently pressed.
		static bool isMouseButtonPressed(int mouseButton); //!< Returns a bool based on whether the specific mouse button is currently pressed.
		static glm::vec2 getMousePosition(); //!< Returns the current mouse position as a glm::vec2.
		static void setCurrentWindow(GLFWwindow* window) { s_window = window; } //!< Setting the current window. 
	private:
		static GLFWwindow* s_window; //!< Current GLFW window.
	};
}