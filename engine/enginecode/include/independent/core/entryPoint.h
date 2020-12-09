/**\ file entryPoint.h */
#pragma once

#include "core/application.h"

extern Engine::Application* Engine::startApplication();

/**\ Entrypoint to the application
*	 
*/
int main(int argc, char** argv)
{
	auto application = Engine::startApplication(); //!< Creates an application object
	application->run(); //!< Calls application.run() which subsiquently initiallizes the necessary code, before running the event loop
	delete application; //!< Once the event loop has closed in application, and it can close, entrypoint deletes application.

	return 0; //!< Closes the program
}

