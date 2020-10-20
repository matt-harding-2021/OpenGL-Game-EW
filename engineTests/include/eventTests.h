#pragma once
#include <gtest/gtest.h>

#include "systems/events/event.h"
#include "systems/events/eventDispatcher.h"
#include "systems/events/eventKeyPressed.h"
#include "systems/events/eventKeyReleased.h"
#include "systems/events/eventKeyTyped.h"
#include "systems/events/eventMouseButtonPressed.h"
#include "systems/events/eventMouseButtonReleased.h"
#include "systems/events/eventMouseMoved.h"
#include "systems/events/eventMouseScrolled.h"
#include "systems/events/eventWindowClose.h"
#include "systems/events/eventWindowFocus.h"
#include "systems/events/eventWindowLostFocus.h"
#include "systems/events/eventWindowMoved.h"
#include "systems/events/eventWindowResize.h"

namespace Engine {
	class EngineTests
	{
	public:

	private:
		const int m_width = 1024;
		const int m_height = 720;

		Engine::KeyPressed keypressed;
		Engine::KeyReleased keyreleased;
		Engine::KeyTyped keytyped;
		Engine::MouseButtonPressed mousepressed;
		Engine::MouseButtonReleased mousereleased;
		Engine::MouseMoved mousemoved;
		Engine::MouseScrolled mousescrolled;
		Engine::WindowClose close;
		Engine::WindowFocus focus;
		Engine::WindowLostFocus windowlostfocus;
		Engine::WindowMoved moved;
		//Engine::WindowResize resize(m_width, m_height);
	};
}