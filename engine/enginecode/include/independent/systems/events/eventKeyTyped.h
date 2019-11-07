/** \file eventKeyTyped.h
*/

#pragma once

#include "event.h"
/**
\class Handling key typed
*/

namespace Engine {

	class KeyTyped : public Event
	{
	private:
		int m_KeyCode;
	public:
		KeyTyped(int keycode) : m_KeyCode(keycode) {} //!<

		virtual EventType getEventType() const override { return EventType::KeyTyped; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard; }
	};
}