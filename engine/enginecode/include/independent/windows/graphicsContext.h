/** \file graphicsContext
*/

#pragma once

/**
\class Interface class for all graphics contexts. Will combine windows tech with graphics API i.e. OPENGLGLFW or WIN32DIRECTX
* Basically provides means for initialisation and swapchain implementation.
*/

class GraphicsContext
{
public:
	virtual void init() = 0; //!< Initialise the graphics context for the given windowing API
	virtual void swapBuffers() = 0; //!< Swap the front and back buffers (Double buffering)
};