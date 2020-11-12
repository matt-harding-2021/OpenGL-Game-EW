/* \file inputPoller */
#include "engine_pch.h"
#include "events/inputPoller.h"

#ifdef NG_PLATFORM_WINDOWS
#include "platform/windows/GLFWInputPoller.h"
#endif
namespace Engine {
#ifdef NG_PLATFORM_WINDOWS
	bool InputPoller::isKeyPressed(int keyCode)
	{
		return GLFWInputPoller::isKeyPressed(keyCode);
	}
	bool InputPoller::isMouseButtonPressed(int mouseButton)
	{
		return GLFWInputPoller::isMouseButtonPressed(mouseButton);
	}
	glm::vec2 InputPoller::getMousePosition()
	{
		return GLFWInputPoller::getMousePosition();
	}
	void InputPoller::setNativeWindow(void* nativeWindow)
	{
		GLFWInputPoller::setCurrentWindow(reinterpret_cast<GLFWwindow*>(nativeWindow));
	}
#endif
}