/** \file application.h
*/
#pragma once

#include "core/macros.h"
#include "systems/log.h"
#include "systems/timer.h"
#include "systems/windows.h"
#include "systems/layerStack.h"
#include "interfaces/IWindow.h"
#include "events/windowsEvents.h"
#include "events/keyEvents.h"
#include "rendering/resourceManager.h"

// Temp include move to a 3D layer
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		bool m_running = true; //!< Application running status
		std::shared_ptr<IWindow> m_window; //!< Window (abstract)
		static glm::ivec2 s_screenResolution; //!< Screen res
		static float s_timestep; //!< Last frames timestep
		// Systems
		std::shared_ptr<Log> m_log; //!< Logging system
		static std::shared_ptr<ResourceManager> s_resources; //!< Resource managemnt system
		std::shared_ptr<Timer> m_timer; //!< Timing system
		std::shared_ptr<Windows> m_windows; //!< Windows system
		std::shared_ptr<LayerStack> m_layerStack; //!< Layer Stack	
		// Event handling functions
		bool onClose(WindowCloseEvent& e); //!< On close event (abstracted)
		bool onResize(WindowResizeEvent& e); //!< On resize event
		bool onKeyPressed(KeyPressedEvent& e); //!< On krepress event
	public:
		virtual ~Application(); //!< Deconstructor
		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
		void run(); //!< Main loop
		void onEvent(Event& e); //!< Called when an OS event happens
		void pushLayer(const std::shared_ptr<Layer>& layer); //!< Push layer on to the stack
		void popLayer(); //!< Pop layer off the stack
		static inline float getPreviousTimestep() { return s_timestep; }
		static inline glm::ivec2 getScreenResolution() { return s_screenResolution; } //!< Returns the screen res of the game
		static inline std::shared_ptr<ResourceManager> getResources() { return s_resources; } //!< Return resource manager
	};

	// To be defined in users code
	Application* startApplication(); //!< Function definition which provides an entry hook

}