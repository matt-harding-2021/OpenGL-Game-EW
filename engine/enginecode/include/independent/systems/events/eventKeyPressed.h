/** \file eventKeyPressed.h
*/

#pragma once

#include "event.h"

/**
\class Handling key presses
*/

namespace Engine {

	class KeyPressed : public Event
	{
	private:
		int m_KeyCode;
		int m_RepeatCount;
	public:
		KeyPressed(int keycode, int repeatcount) : m_KeyCode(keycode), m_RepeatCount(repeatcount) {} //!<
		inline int getRepeateCount() const { return m_RepeatCount; }

		virtual EventType getEventType() const override { return EventType::KeyPressed; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard; }
	};
}