#pragma once
#include "systems/system.h"
#include <GLFW/glfw3.h>
namespace Engine {
	class GLFWWindowsSystem : public System
	{
	public:
		GLFWWindowsSystem();
		~GLFWWindowsSystem();

		void start(SystemSignal init = SystemSignal::None, ...) override { glfwInit(); }
		void stop(SystemSignal close = SystemSignal::None, ...) override { glfwTerminate(); }
	private:

	};

	GLFWWindowsSystem::GLFWWindowsSystem()
	{
	}

	GLFWWindowsSystem::~GLFWWindowsSystem()
	{
	}
}