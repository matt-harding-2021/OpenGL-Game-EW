/** \file eventMouseScrolled.h
*/

#pragma once

#include "event.h"

/**
\class Handling the mouse wheel being scrolled
*/

namespace Engine {

	class MouseScrolled : public Event
	{
	private:

	public:
		MouseScrolled() {} //!<
		static EventType getStaticType() { return EventType::MouseScrolled; }
		virtual EventType getEventType() const override { return EventType::MouseScrolled; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }
	};
}