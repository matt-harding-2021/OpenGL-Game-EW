/** \file eventWindowLostFocus.h
*/

#pragma once

#include "event.h"

/**
\class Handling functions on losing focus of the window
*/

namespace Engine {

	class WindowLostFocus : public Event
	{
	private:
	public:
		WindowLostFocus() {} //!<

		virtual EventType getEventType() const override { return EventType::WindowLostFocus; }
		virtual int getCategoryFlags() const override { return EventCategoryWindow; }
	};
}