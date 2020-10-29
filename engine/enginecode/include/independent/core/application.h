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


#include "windows/window.h"

namespace Engine {

	/**
	\class Application
	Fundemental class of the engine. A singleton which runs the game loop infinitely.
	Provides ...
	*/

	class Application
	{
	protected:
		Application(); //!< Constructor
	private:
		static Application* s_instance; //!< Singleton instance of the application
		std::shared_ptr<logging> m_Log;
		std::shared_ptr<timer> m_Timer;


		std::shared_ptr<System> m_windowsSystem;
		std::unique_ptr<Window> m_Window;

		bool bRunning = true;
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void onEvent(Event& e); //!< Called when an event happens

		bool onKeyPressed(KeyPressedEvent& e);
		bool onKeyReleased(KeyReleasedEvent& e);
		bool onKeyTyped(KeyTypedEvent& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonPressedEvent& e);
		bool onMouseMoved(MouseMovedEvent& e);
		bool onMouseScrolled(MouseScrolledEvent& e);
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowFocus(WindowFocusEvent& e);
		bool onWindowLostFocus(WindowLostFocusEvent& e);
		bool onWindowMoved(WindowMovedEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}