#pragma once

#include "event.h"

namespace Engine {

	class KeyEvent : public Event
	{
	protected:
		KeyEvent(int keycode) : m_keyCode(keycode) {}
		int m_keyCode;
	public:
		inline int GetKeyCode() const { return m_keyCode; }
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }
	};

	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_repeatCount;
	public:
		KeyPressedEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_repeatCount; }
		static EventType getStaticType() { return EventType::KeyPressed; }
		virtual EventType getEventType() const override { return getStaticType(); }
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		static EventType getStaticType() { return EventType::KeyPressed; }
		virtual EventType getEventType() const override { return getStaticType(); }
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		static EventType getStaticType() { return EventType::KeyTyped; }
		virtual EventType getEventType() const override { return getStaticType(); }
	};
}