/** \file eventMouseButtonPressed.h
*/

#pragma once

#include "event.h"

/**
\class Handling each mouse button being pressed
*/

namespace Engine {

	class MouseButtonPressed :public Event
	{
	private:
		
	public:
		MouseButtonPressed() {} //!<

		virtual EventType getEventType() const override { return EventType::MouseButtonPressed; }
		virtual int getCategoryFlags() const override { return EventCategoryMouseButton; }
	};
}