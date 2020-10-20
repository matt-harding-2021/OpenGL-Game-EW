#pragma once

#include "event.h"
#include <glm/glm.hpp>

namespace Engine {

	class MouseButtonPressedEvent : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonPressedEvent(int button) : m_button(button) {}

		static EventType getStaticType() { return EventType::MouseButtonPressed; }
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
		inline float getButton() const { return m_button; }
	};

	class MouseButtonReleasedEvent : public Event
	{
	private:
		int m_button;
	public:
		MouseButtonReleasedEvent(int button) : m_button(button) {}

		static EventType getStaticType() { return EventType::MouseButtonReleased; }
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
		inline float getButton() const { return m_button; }
	};

	class MouseMovedEvent : public Event
	{
	private:
		float m_mouseX, m_mouseY;
	public:
		MouseMovedEvent(float x, float y) : m_mouseX(x), m_mouseY(y) {}

		static EventType getStaticType() { return EventType::MouseMoved; }
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
		inline float getX() const { return m_mouseX; }
		inline float getY() const { return m_mouseY; }
		inline glm::ivec2 getPos() const { return glm::ivec2(m_mouseX, m_mouseY); }
	};

	class MouseScrolledEvent : public Event
	{
	private:
		float m_xOffset, m_yOffset;
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

		static EventType getStaticType() { return EventType::MouseScrolled; }
		virtual EventType getEventType() const override { return getStaticType(); }
		virtual int getCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
		inline float getXOffset() const { return m_xOffset; }
		inline float getYOffset() const { return m_yOffset; }
	};

}