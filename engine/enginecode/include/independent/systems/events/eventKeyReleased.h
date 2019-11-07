/** \file eventKeyReleased.h
*/

#pragma once

#include "event.h"

/**
\class Handling key releases
*/

namespace Engine {

	class KeyReleased : public Event
	{
	private:
		int m_KeyCode;
	public:
		KeyReleased(int keycode) : m_KeyCode(keycode) {} //!<

		virtual EventType getEventType() const override { return EventType::KeyReleased; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard; }
	};
}