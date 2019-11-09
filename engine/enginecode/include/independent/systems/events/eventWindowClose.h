/** \file eventWindowClose.h
*/

#pragma once

#include "event.h"

/**
\class Handling closing the window
*/

namespace Engine {

	class WindowClose : public Event
	{
	private:
		//No data needed
	public:
		WindowClose() {} //!<
		static EventType getStaticType() { return EventType::WindowClose; }
		EventType getEventType() const override { return EventType::WindowClose; }
		int getCategoryFlags() const override { return EventCategoryWindow; }
	};
}