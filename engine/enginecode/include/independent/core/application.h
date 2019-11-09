/** \file application.h
*/
#pragma once

#include <memory>

#include "systems/logging.h"
#include "systems/timer.h"
#include "systems/events/event.h"
#include "systems/events/eventDispatcher.h"

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

		bool bRunning = true;
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void onEvent(Event& e); //!< Called when an event happens
		bool onKeyPress(KeyPressed& e);
		bool onKeyRelease(KeyReleased& e);
		bool onKeyTyped(KeyTyped& e);
		bool onMouseButtonPress(MouseButtonPressed& e);
		bool onMouseButtonRelease(MouseButtonReleased& e);
		bool onMouseMove(MouseMoved& e);
		bool onMouseScroll(MouseScrolled& e);
		bool onWindowClose(WindowClose& e);
		bool onWindowFocus(WindowFocus& e);
		bool onWindowLostFocus(WindowLostFocus& e);
		bool onWindowMove(WindowMoved& e);
		bool onWindowResize(WindowResize& e);
		void run(); //!< Main loop
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}