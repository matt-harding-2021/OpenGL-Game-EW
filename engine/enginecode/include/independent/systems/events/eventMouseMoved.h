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
		int m_xPos;
		int m_yPos;
	public:
		MouseMoved(int xPos, int yPos) : m_xPos(xPos), m_yPos(yPos) {} //!<
		static EventType getStaticType() { return EventType::MouseMoved; }
		virtual EventType getEventType() const override { return EventType::MouseMoved; }
		virtual int getCategoryFlags() const override { return EventCategoryMouse; }
	};
}