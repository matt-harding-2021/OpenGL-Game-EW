/**\ file keyEvents.h */
#pragma once
#include "event.h"

namespace Engine {

	/**\ Class KeyEvent 
	*	 The keycode is initialised here, as it is used in all subsequent classes
	*/
	class KeyEvent : public Event
	{
	protected:
		KeyEvent(int arg_keycode) : m_keyCode(arg_keycode) {} //!< Constructor initializes m_keycode to the event key
		int m_keyCode;
	public:
		inline int GetKeyCode() const { return m_keyCode; } //!< Returns the specific key 
		virtual int getCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; } //!< Returns the event categories a key event falls into
	};

	/**\ Class KeyPressedEvent. Inherits from KeyEvent */
	class KeyPressedEvent : public KeyEvent
	{
	private:
		int m_repeatCount;
	public:
		KeyPressedEvent(int arg_keycode, int arg_repeatCount) : KeyEvent(arg_keycode), m_repeatCount(arg_repeatCount) {} //!< Constructor initializes the keycode in keyevent class, and m_repeatcount

		inline int GetRepeatCount() const { return m_repeatCount; } //!< Returns whetheer or not the key is held down
		static EventType getStaticType() { return EventType::KeyPressed; } //!< As it is a more specific event, it need a getType() function to return the type of event
		virtual EventType getEventType() const override { return getStaticType(); }
	};

	/**\ Class KeyReleasedEvent. Inherits from KeyEvent */
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int arg_keycode) : KeyEvent(arg_keycode) {} //!< Constructor initializes the keycode in keyevent class

		static EventType getStaticType() { return EventType::KeyReleased; }
		virtual EventType getEventType() const override { return getStaticType(); }
	};

	/**\ Class KeyTypedEvent. Inherits from KeyEvent */
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int arg_keycode) : KeyEvent(arg_keycode) {} //!< Constructor initializes the keycode in keyevent class

		static EventType getStaticType() { return EventType::KeyTyped; } //!< Returns the specific type of event 
		virtual EventType getEventType() const override { return getStaticType(); }
	};
}