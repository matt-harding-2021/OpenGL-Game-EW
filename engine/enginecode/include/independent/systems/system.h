/** \file system.h
*/

#pragma once

#include <cstdarg>

/**
\class Interface class for all systems
*/

namespace Engine {

	class system
	{
	public:
		virtual ~system() {};
		virtual void start(unsigned int argc = 0, ...) = 0; //!< Start the system
		virtual void stop(unsigned int argc = 0, ...) = 0; //!< Stop the system
	};
}
