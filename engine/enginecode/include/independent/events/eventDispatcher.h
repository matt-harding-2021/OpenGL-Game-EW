/**\ file eventDispatcher.h */
#pragma once
#include "event.h"

namespace Engine {
	/**\ Class Event dispatcher
	*	 
	*/
	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;
	private:
		Event& m_event;
	public:
		EventDispatcher(Event& arg_event) : m_event(arg_event) {}
		
		template<typename T>
		bool dispatch(EventFunc<T> arg_func)
		{
			if (m_event.getEventType() == T::getStaticType())
			{
				m_event.handle(arg_func(*((T*)&m_event)));
				return true;
			}
			return false;
		}
	};
}