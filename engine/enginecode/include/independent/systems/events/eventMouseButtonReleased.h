/** \file eventMouseButtonReleased.h
*/

#pragma once

#include "event.h"

/**
\class Handling each mouse button being released
*/

namespace Engine {

	class MouseButtonReleased : public Event
	{
	private:
		int m_MouseButton;
	public:
		MouseButtonReleased() {} //!<
		static EventType getStaticType() { return EventType::MouseButtonReleased; }
		virtual EventType getEventType() const override { return EventType::MouseButtonReleased; }
		virtual int getCategoryFlags() const override { return EventCategoryMouseButton; }
	};
}