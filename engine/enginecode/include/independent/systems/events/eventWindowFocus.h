/** \file eventWindowFocus.h
*/

#pragma once

#include "event.h"

/**
\class Handling functions on focus of the window
*/

namespace Engine {

	class WindowFocus : public Event
	{
	private:
	public:
		WindowFocus() {} //!<

		virtual EventType getEventType() const override { return EventType::WindowFocus; }
		virtual int getCategoryFlags() const override { return EventCategoryWindow; }
	};
}