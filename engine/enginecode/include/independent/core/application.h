/** \file application.h
*/
#pragma once

#include <memory>

#include "systems/logging.h"
#include "systems/timer.h"

#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/keyEvents.h"
#include "events/mouseEvents.h"
#include "events/windowEvents.h"
#include "events/codes.h"
#include "events/inputPoller.h"

#include "windows/window.h"

namespace Engine {

	/**
	\class Application
	Fundemental class of the engine. A singleton which runs the game loop infinitely.
	Provides
		Event management
		Window implementation 
		OpenGL context and Rendering
		...
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor

		std::shared_ptr<logging> m_Log; //!< Logging object that can output information to the console
		std::shared_ptr<timer> m_Timer; //!< Timer object records timeframes and calculates timesteps


		std::shared_ptr<System> m_windowsSystem; //!< 
		std::unique_ptr<Window> m_Window; //!< A single instance of a window


		void onEvent(Event& e); //!< Called when an event happens
		/**\ Key events */
		bool onKeyPressed(KeyPressedEvent& e);
		bool onKeyReleased(KeyReleasedEvent& e);
		bool onKeyTyped(KeyTypedEvent& e);
		/**\ Mouse events */
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool onMouseMoved(MouseMovedEvent& e);
		bool onMouseScrolled(MouseScrolledEvent& e);
		/**\ Window events */
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowFocus(WindowFocusEvent& e);
		bool onWindowLostFocus(WindowLostFocusEvent& e);
		bool onWindowMoved(WindowMovedEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		static Application* s_instance; //!< Singleton instance of the application
		bool m_Running = true; //!< Bool controls application loop

		bool m_mouseButton1Pressed = false;
		glm::vec2 m_mousePosCurrent = { 0,0 };
		glm::vec2 m_mousePosStart = { 0,0 };

	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Returns instance from singleton pattern
		void run(); //!< Main loop
	};

	Application* startApplication(); //!< Function definition which provides an entry hook
}