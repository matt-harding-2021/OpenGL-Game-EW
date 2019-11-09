/** \file eventWindowResize.h
*/

#pragma once

#include "event.h"

/**
\class Handling resizing the window
*/

namespace Engine {

	class WindowResize : public Event
	{
	private:
		int m_Width; //<! New window size in x axis
		int m_Height; //<! New window size in y axis
	public:
		WindowResize(int width, int height) : m_Width(width), m_Height(height) {} //!<
		static EventType getStaticType() { return EventType::WindowResize; }
		virtual EventType getEventType() const override { return EventType::WindowResize; }
		virtual int getCategoryFlags() const override { return EventCategoryWindow; }

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }
	};
}