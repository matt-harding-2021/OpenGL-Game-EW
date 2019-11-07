/** \file eventMouseMoved.h
*/

#pragma once

#include "event.h"

/**
\class Handling events on the mouse being moved
*/

namespace Engine {

	class MouseMoved : public Event
	{
	private:

	public:
		MouseMoved() {} //!<

		virtual EventType getEventType() const override { return EventType::MouseMoved; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }
	};
}