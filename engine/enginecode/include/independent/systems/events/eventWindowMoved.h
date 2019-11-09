/** \file eventWindowMoved.h
*/

#pragma once

#include "event.h"
/**
\class Handling movement of the window
*/

namespace Engine {

	class WindowMoved : public Event
	{
	private:
	public:
		WindowMoved() {} //!<
		static EventType getStaticType() { return EventType::WindowMoved; }
		virtual EventType getEventType() const override { return EventType::WindowMoved; }
		virtual int getCategoryFlags() const override { return EventCategoryWindow; }
	};
}