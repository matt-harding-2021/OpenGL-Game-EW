/** \file event.h
*/

#pragma once

/**
\class Handling general attributes of all events
*/

namespace Engine {

	enum class EventType 
	{ 
		None = 0, 
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory 
	{ 
		None = 0, 
		EventCategoryWindow = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4,
	};

	class Event
	{
	private:
		bool m_handled = false;
	public:
		virtual EventType getEventType() const = 0; //!< Get the event type
		virtual int getCategoryFlags() const = 0; //!< Get the event category
		inline bool handled() const { return m_handled; } //!< Has the event been handled
		inline void handle(bool isHandled) { m_handled = isHandled; } //!< Handles the event
		inline bool isInCategory(EventCategory category) { return getCategoryFlags() & category; } //!< Is this event in the category?
	};
	//Not a pure interface
	//It's abstract
}