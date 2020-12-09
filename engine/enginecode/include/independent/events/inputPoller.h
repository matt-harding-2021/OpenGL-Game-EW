/**\ file inputPoller.h */
#pragma once
#include <glm/glm.hpp>

namespace Engine {

	/* \class InputPoller
	*	API agnostic input poller for getting current keyboard/mouse state
	*/
	class InputPoller
	{
	public:
		static bool isKeyPressed(int keyCode); //!< Returns a bool based on whether the specific key code is currently pressed.
		static bool isMouseButtonPressed(int mouseButton); //!< Returns a bool based on whether the specific mouse button is currently pressed.
		static glm::vec2 getMousePosition(); //!< Returns the current mouse position as a glm::vec2.
		static void setNativeWindow(void* nativeWindow); //!< Sets the native window for the input poller to poll from.
	private:

	};
}