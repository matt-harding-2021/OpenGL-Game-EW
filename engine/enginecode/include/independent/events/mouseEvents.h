/**\ file mouseEvents.h */
#pragma once

#include "event.h"
#include <glm/glm.hpp>

namespace Engine {

	/**\ Class MouseButtonPressedEvent. Inherits from Class Event */
	class MouseButtonPressedEvent : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonPressedEvent(int button) : m_button(button) {} //!< Constructor initializes m_button to the mouse button

		static EventType getStaticType() { return EventType::MouseButtonPressed; } //!< Returns the specific type of event 
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; } //!< Returns the event categories a mouse event falls into
		inline float getButton() const { return m_button; } //!< Returns the specific mouse button
	};

	/**\ Class MouseButtonReleasedEvent. Inherits from Class Event */
	class MouseButtonReleasedEvent : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonReleasedEvent(int button) : m_button(button) {} //!< Constructor initializes m_button to the mouse button

		static EventType getStaticType() { return EventType::MouseButtonReleased; } //!< Returns the specific type of event 
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; } //!< Returns the event categories a mouse event falls into
		inline float getButton() const { return m_button; } //!< Returns the specific mouse button
	};

	class MouseMovedEvent : public Event
	{
	private:
		float m_mouseX, m_mouseY;
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {} //!< Constructor initializes m_mouseX and m_mouseY to the mouse x and y positions.

		static EventType getStaticType() { return EventType::MouseMoved; } //!< Returns the specific type of event 
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; } //!< Returns the event categories a mouse event falls into
		inline float getX() const { return m_mouseX; }
		inline float getY() const { return m_mouseY; }
		inline glm::ivec2 getPos() const { return glm::ivec2(m_mouseX, m_mouseY); } //!< Returns the mouse co-ordinates as a vector with x and y
	};

	class MouseScrolledEvent : public Event
	{
	private:
		float m_offsetX, m_offsetY;
	public:
		MouseScrolledEvent(float arg_offsetX, float arg_offsetY) : m_offsetX(arg_offsetX), m_offsetY(arg_offsetY) {} //!< Constructor initializes m_offsetX and m_offsetY to the scroll direction passed.

		static EventType getStaticType() { return EventType::MouseScrolled; } //!< Returns the specific type of event 
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; } //!< Returns the event categories a mouse event falls into
		inline float getXOffset() const { return m_offsetX; } //!< Returns the scroll direction in the x axis
		inline float getYOffset() const { return m_offsetY; } //!< Returns the scroll direction in the y axis (most common)
	};

}