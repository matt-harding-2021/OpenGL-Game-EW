//LOOK UP EVENTDISPATCHER.H TEMPLATES//

/** \file application.cpp */
#include "engine_pch.h"
#include "core/application.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef NG_PLATFORM_WINDOWS
#include "platform/windows/GLFWWindowsSystem.h"
#include "platform/windows/GLFWWindowImpl.h"
#endif
#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/uniformBuffer.h"
#include "rendering/shader.h"
#include "rendering/texture.h"
#include "rendering/subTexture.h"

#include "rendering/renderer3D.h"	
#include "rendering/renderer2D.h"

#include <string>

namespace Engine {

	Application* Application::s_instance = nullptr; //!< Single instance of application ensures only one can be open at a time

	 /** Constructor for the application class
	 *	 Involves initialising the different aspects of the application class:
	 *		The window 
	 *		Event handlers (polling and dispatching)
	 *		OpenGL rendering
	 */
	Application::Application()
	{
		/**\ Sets the static instance to this object */
		if (s_instance == nullptr)
		{
			s_instance = this;
		}

		/**\ Creating new instances of the logger and timer
		*	 Using shared pointers means we don't have to worry about it freeing up memory at end.
		*/
		m_Log = std::make_shared<logging>();  
		m_Log->start(); 

		m_Timer = std::make_shared<timer>();
		m_Timer->start();

#ifdef NG_PLATFORM_WINDOWS //!< If the platform used is windows, we can use OpenGL as it is supported 
		m_windowsSystem = std::make_shared<GLFWWindowsSystem>(); 
		m_windowsSystem->start(); //!< Calls GLFWInit() from the GLFW library, this lets us use openGL

		WindowProperties winProps; //!< Generates the window properties such as height and width
		m_Window = std::make_unique<GLFWWindowImpl>(winProps); //!< Using the window properties, create a windows implementation. This is an abstracted class that creates an openGL window

		/**\ Along with creating windows, opengl lets us handle user events */
		m_Window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1)); //!< Uses the onEvent function whenever opengl detects an event
		InputPoller::setNativeWindow(m_Window->getNativeWindow());  //!< Refers which specific window we want the events to be polled at
#endif

	}

	/**\ Very simple clean-up of the different systems used */
	Application::~Application()
	{
		m_windowsSystem->stop();
		m_windowsSystem.reset();

		m_Timer->stop();
		m_Timer.reset();
		
		m_Log->stop();
		m_Log.reset();
	}

	/** Used as the glfw event callback
	*	 onEvent creates a temporary dispatcher that checks through the possible eventTypes and runs the bound function if it matches.
	*	 This method is for functions that should happen every time a specific event occurs, such as window resizing and closing
	*/
	void Application::onEvent(Event & e)
	{
		EventDispatcher dispatcher(e); //!< Creating the dispatcher with reference to the event called
		
		/**\ Key Events */
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		dispatcher.dispatch<KeyReleasedEvent>(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));

		/**\ Mouse Events */
		dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&Application::onMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.dispatch<MouseButtonReleasedEvent>(std::bind(&Application::onMouseButtonReleased, this, std::placeholders::_1));
		dispatcher.dispatch<MouseMovedEvent>(std::bind(&Application::onMouseMoved, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrolledEvent>(std::bind(&Application::onMouseScrolled, this, std::placeholders::_1));

		/**\ Window Events */
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowFocusEvent>(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowLostFocusEvent>(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowMovedEvent>(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		
	}
	/**\ Functions the dispatcher calls depending on event type */
	bool Application::onKeyPressed(KeyPressedEvent& e) { 
		//if (e.GetRepeatCount() == 0) LOG_INFO("Key Pressed: {0}", e.GetKeyCode());
		//else if (e.GetRepeatCount() == 1) LOG_INFO("Key Repeated: {0}", e.GetKeyCode());
		return true; 
	}
	bool Application::onKeyReleased(KeyReleasedEvent& e) { 
		//LOG_INFO("Key Released: {0}", e.GetKeyCode());
		return true; 
	}
	bool Application::onKeyTyped(KeyTypedEvent& e) { return true; } 
	bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) { 
		//LOG_INFO("Mouse Button Pressed: {0}", e.getButton());
		return true;
	}
	bool Application::onMouseButtonReleased(MouseButtonReleasedEvent& e) {
		//LOG_INFO("Mouse Button Released: {0}", e.getButton());
		return true;
	}
	bool Application::onMouseMoved(MouseMovedEvent& e) { 
		m_mousePosCurrent = glm::vec2(e.getPos().x, e.getPos().y); //!< Updates the current position every time the mouse is moved
		//LOG_INFO("Mouse Moved: {0}x{1}", e.getPos().x, e.getPos().y);
		return true; 
	}
	bool Application::onMouseScrolled(MouseScrolledEvent& e) { 
		//LOG_INFO("Mouse Scrolled: {0}x{1}", e.getXOffset(), e.getYOffset());
		return true; 
	}
	bool Application::onWindowClose(WindowCloseEvent& e) {
		//LOG_INFO("Window Closed");
		m_Running = false;  //!< Set the application loop boolean to false
		return true;
	}
	bool Application::onWindowFocus(WindowFocusEvent& e) { 
		//LOG_INFO("Window Focused");
		return true; 
	}
	bool Application::onWindowLostFocus(WindowLostFocusEvent& e) { 
		//LOG_INFO("Window Lost Focus");
		return true; 
	}
	bool Application::onWindowMoved(WindowMovedEvent& e) { 
		//LOG_INFO("Window Moved: {0}x{1}", e.getPos().x, e.getPos().y);
		return true; 
	}
	bool Application::onWindowResize(WindowResizeEvent& e) {
		//LOG_INFO("Window Resized: {0}x{1}", e.getWidth(), e.getHeight());
		return true;
	}


	/**	Once the app is initialised in entrypoint.h, app.run() begins the actual program. Contains: 
	*		OpenGL setup (in API agnostic classes)
	*		Event loop with input polling
	*		Per frame update functions 
	*		Including rendering
	*/
	void Application::run()
	{
#pragma region TEXTURES
		/**	Implementing the abstracted OpenGL Textures	*/
		std::shared_ptr<Texture> textureAtlas;
		textureAtlas.reset(Texture::create("assets/textures/letterAndNumberCube(Dark).png")); //!< Creates a new texture using a png image from the filepath given. (I made a darker texture to hopefully show the phong lighting better)
		SubTexture letterTexture(textureAtlas, { 0.f, 0.f }, { 1.f, 0.5f }); //!< Finds the letter texture within the texture atlas using UV coordinates
		SubTexture numberTexture(textureAtlas, { 0.f, 0.5f }, { 1.f, 1.f }); //!< Finds the number texture within the texture atlas using UV coordinates

		std::shared_ptr<Texture> pyramidTex;
		unsigned char PxlColour[4] = { 155, 0, 55, 255 };
		pyramidTex.reset(Texture::create(1, 1, 4, PxlColour));

		std::shared_ptr<Texture> gearTexture;
		gearTexture.reset(Texture::create("assets/textures/gear.png")); //!< Creates a new texture using a png image from the filepath given. (I made a darker texture to hopefully show the phong lighting better)
#pragma endregion


#pragma region RAW_DATA

		float cubeVertices[8 * 24] = {
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,	numberTexture.transformUV(0.f,   0.f).x,	numberTexture.transformUV(0.f,   0.f).y,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,	numberTexture.transformUV(0.f,   0.5f).x,	numberTexture.transformUV(0.f,   0.5f).y,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,	numberTexture.transformUV(0.33f, 0.5f).x,	numberTexture.transformUV(0.33f, 0.5f).y,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,	numberTexture.transformUV(0.33f, 0.f).x,	numberTexture.transformUV(0.33f, 0.f).y,
				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,	numberTexture.transformUV(0.33f, 0.5f).x,	numberTexture.transformUV(0.33f, 0.5f).y,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,	numberTexture.transformUV(0.66f, 0.5f).x,	numberTexture.transformUV(0.66f, 0.5f).y,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,	numberTexture.transformUV(0.66f, 0.f).x,	numberTexture.transformUV(0.66f, 0.f).y,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,	numberTexture.transformUV(0.33,  0.f).x,	numberTexture.transformUV(0.33,  0.f).y,

				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,	numberTexture.transformUV(1.f,   0.f).x,	numberTexture.transformUV(1.f,   0.f).y,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,	numberTexture.transformUV(0.66f, 0.f).x,	numberTexture.transformUV(0.66f, 0.f).y,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,	numberTexture.transformUV(0.66f, 0.5f).x,	numberTexture.transformUV(0.66f, 0.5f).y,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,	numberTexture.transformUV(1.0f,  0.5f).x,	numberTexture.transformUV(1.0f,  0.5f).y,

				 0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	numberTexture.transformUV(0.f,   0.5f).x,	numberTexture.transformUV(0.f,   0.5f).y,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,	numberTexture.transformUV(0.f,   1.0f).x,	numberTexture.transformUV(0.f,   1.0f).y,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,	numberTexture.transformUV(0.33f, 1.0f).x,	numberTexture.transformUV(0.33f, 1.0f).y,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	numberTexture.transformUV(0.3f,  0.5f).x,	numberTexture.transformUV(0.33f,  0.5f).y,

				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,	numberTexture.transformUV(0.66f, 0.5f).x,	numberTexture.transformUV(0.66f, 0.5f).y,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,	numberTexture.transformUV(0.33f, 0.5f).x,	numberTexture.transformUV(0.33f, 0.5f).y,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,	numberTexture.transformUV(0.33f, 1.0f).x,	numberTexture.transformUV(0.33f, 1.0f).y,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,	numberTexture.transformUV(0.66f, 1.0f).x,	numberTexture.transformUV(0.66f, 1.0f).y,

				 0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,	numberTexture.transformUV(1.0f,  1.0f).x,	numberTexture.transformUV(1.0f,  1.0f).y,
				 0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,	numberTexture.transformUV(1.0f,  0.5f).x,	numberTexture.transformUV(1.0f,  0.5f).y,
				 0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,	numberTexture.transformUV(0.66f, 0.5f).x,	numberTexture.transformUV(0.66f, 0.5f).y,
				 0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,	numberTexture.transformUV(0.66f, 1.0f).x,	numberTexture.transformUV(0.66f, 1.0f).y
		};

		float pyramidVertices[8 * 16] = {
			//	 <------ Pos ------>		<--- normal --->			<-- UV --> 
				-0.5f, -0.5f, -0.5f,		0.f, -1.f, 0.f,				0.f, 0.f,		// square Magneta
				 0.5f, -0.5f, -0.5f,		0.f, -1.f, 0.f,				0.f, 0.25f,
				 0.5f, -0.5f,  0.5f,		0.f, -1.f, 0.f,				0.33f, 0.25f,
				-0.5f, -0.5f,  0.5f,		0.f, -1.f, 0.f,				0.33f, 0.f,

				-0.5f, -0.5f, -0.5f,		-0.8944f, 0.4472f, 0.f,		0.33f, 0.25f,		// triangle Green
				-0.5f, -0.5f,  0.5f,		-0.8944f, 0.4472f, 0.f,		0.66f, 0.25f,
				 0.0f,  0.5f,  0.0f,		-0.8944f, 0.4472f, 0.f,		0.5f, 0.f,

				-0.5f, -0.5f,  0.5f,		0.f, 0.4472f, 0.8944f,		0.f, 0.f,		// triangle Red
				 0.5f, -0.5f,  0.5f,		0.f, 0.4472f, 0.8944f,		0.f, 0.f,
				 0.0f,  0.5f,  0.0f,		0.f, 0.4472f, 0.8944f,		0.f, 0.f,

				 0.5f, -0.5f,  0.5f,		0.8944f, 0.4472f, 0.f,		0.f, 0.f,		// triangle Yellow
				 0.5f, -0.5f, -0.5f,		0.8944f, 0.4472f, 0.f,		0.f, 0.f,
				 0.0f,  0.5f,  0.0f,		0.8944f, 0.4472f, 0.f,		0.f, 0.f,

				 0.5f, -0.5f, -0.5f,		0.f, 0.4472f, -0.8944f,		0.f, 0.f,		// triangle Blue
				-0.5f, -0.5f, -0.5f,		0.f, 0.4472f, -0.8944f,		0.f, 0.f,
				 0.0f,  0.5f,  0.0f,		0.f, 0.4472f, -0.8944f,		0.f, 0.f
		};

		uint32_t pyramidIndices[3 * 6] =
		{
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			7, 8, 9,
			10, 11, 12,
			13, 14, 15
		};

		uint32_t cubeIndices[3 * 12] = {
			0, 1, 2,
			2, 3, 0,
			4, 5, 6,
			6, 7, 4,
			8, 9, 10,
			10, 11, 8,
			12, 13, 14,
			14, 15, 12,
			16, 17, 18,
			18, 19, 16,
			20, 21, 22,
			22, 23, 20
		};
#pragma endregion
#pragma region GL_BUFFERS
		/**	Implementing the abstracted frame buffers for OpenGL
		*	Doesn't include any actual openGL library calls.
		*	This means that other rendering libraries could be used in place of openGL in the future.
		*
		*	Rendering buffers allocate memory in the GPU that will be used to hold data about rendering objects (in this instance, a cube and a pyramid).
		*/

		/**\ Creates new abstracted buffer objects for a cube */
		std::shared_ptr<VertexArray> cubeVAO; //!< Vertex Array Object,
		std::shared_ptr<VertexBuffer> cubeVBO; //!< Vertex Buffer Object,
		std::shared_ptr<IndexBuffer> cubeIBO; //!< Index Buffer Object

		cubeVAO.reset(VertexArray::create());

		VertexBufferLayout bufferLayout = { ShaderDataType::Float3, ShaderDataType::Float3, ShaderDataType::Float2 };
		cubeVBO.reset(VertexBuffer::create(cubeVertices, sizeof(cubeVertices), bufferLayout));

		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));

		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);


		std::shared_ptr<VertexArray> pyramidVAO;
		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		pyramidVAO.reset(VertexArray::create());

		pyramidVBO.reset(VertexBuffer::create(pyramidVertices, sizeof(pyramidVertices), bufferLayout));

		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));

		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
#pragma endregion
#pragma region SHADERS
		/**	Implemnting the abstracted OpenGL Shaders
		*	Shader3D uses the Phong lighting model
		*	Each shader object reads a text file and compiles it line by line into the OpenGL library shaders
		*/
		std::shared_ptr<Shader> Shader3D;
		Shader3D.reset(Shader::create("./assets/shaders/Shader3D.glsl"));

#pragma endregion 
#pragma region MATERIALS
		/** Creating the materials
		*	to be used specifically with the renderer3d.
		*	Each material takes a texture, and a shader to be used when rendring.
		*	Taking a shader lets us potentially use different lighting effects on different objects within the 3D world
		*/
		std::shared_ptr<Material> pyramidMaterial;
		pyramidMaterial.reset(new Material(Shader3D, { 1.f, 1.f, 1.f, 1.f }));

		std::shared_ptr<Material> letterCubeMaterial;
		letterCubeMaterial.reset(new Material(Shader3D, textureAtlas));

		std::shared_ptr<Material> numberCubeMaterial;
		numberCubeMaterial.reset(new Material(Shader3D, textureAtlas));
#pragma endregion
#pragma region RENDERERS
		/**\ Renderer3D */
		Renderer3D::init();
		Renderer3D::uploadCamera(
			Shader3D,
			glm::lookAt(					//!< Camera view
				glm::vec3(2.f, 2.f, 0.f),
				glm::vec3(0.f, 0.f, -6.f),
				glm::vec3(0.f, 1.f, 0.f)
			),
			glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f) //!< Camera projection
		);
		Renderer3D::uploadLights(
			Shader3D,
			glm::vec3(2.f, 1.f, -6.f),		//!< Lights position
			glm::vec3(0.f, 0.f, 0.f),		//!< Lights view
			glm::vec3(1.f, 1.f, 1.f),		//!< Lights colour
			glm::vec4(1.f, 1.f, 1.f, 1.f)	//!< Lights tint
		);

		/**\ Renderer2D */
		Renderer2D::init();
		Renderer2D::uploadData(
			glm::mat4(1.f),
			glm::ortho(0.f, static_cast<float>(m_Window->getWidth()), static_cast<float>(m_Window->getHeight()), 0.f)
		);
#pragma endregion 

		std::string fpsStr;
		std::string camStr = std::string("Camera: Top-Right");

		/**\ Setting the model locations */
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.f, -6.f));

		//glClearColor(1.f, 1.f, 1.f, 1.f);
		glClearColor(1.f, 1.f, 1.f, 1.f);
		float elapsedTime = 0;

		/**	The main event loop for the application. Contains:
		*	Event polling
		*	Update functions
		*/
		while (m_Running) {

			timer::startFrameTimer();
			float totalTimeElapsed = timer::getMarkerTimer();

			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1)) {
				glm::vec2 mouseDelta = m_mousePosCurrent - m_mousePosStart;
				if (mouseDelta.x != 0.f)
					//for world coords: save current transform, set transform to (0,0,0), rotate, then transform back
					models[0] = glm::rotate(models[0], elapsedTime * mouseDelta.x, glm::vec3(0.f, 1.f, 0.f));
				if (mouseDelta.y != 0.f)
					models[0] = glm::rotate(models[0], elapsedTime * mouseDelta.y, glm::vec3(1.f, 0.f, 0.f));
			}
			models[2] = glm::rotate(models[2], elapsedTime, glm::vec3(1.f, 1.f, 1.f));
			m_mousePosStart = m_mousePosCurrent;
			
			if (InputPoller::isKeyPressed(NG_KEY_SPACE)) {
				if (!m_spacePressed) {
					switch (m_currentCamPos)
					{
					case 0:
						Renderer3D::uploadCamera(
							Shader3D,
							glm::lookAt(					//!< Top-Left
								glm::vec3(-3.f, 2.f, 0.f),
								glm::vec3(0.f, 0.f, -6.f),
								glm::vec3(0.f, 1.f, 0.f)
							),
							glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f) //!< Camera projection
						);
						camStr = std::string("Camera: Top-Left");
						m_currentCamPos++;
						break;
					case 1: 
						Renderer3D::uploadCamera(
							Shader3D,
							glm::lookAt(					//!< Birds-Eye
								glm::vec3(0.f, 6.f, -5.f),
								glm::vec3(0.f, 0.f, -6.f),
								glm::vec3(0.f, 1.f, 0.f)
							),
							glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f) //!< Camera projection
						);
						camStr = std::string("Camera: Birds-Eye");
						m_currentCamPos++;
						break;
					case 2:
						Renderer3D::uploadCamera(
							Shader3D,
							glm::lookAt(					//!< Centre
								glm::vec3(0.f, 0.f, 0.f),
								glm::vec3(0.f, 0.f, -6.f),
								glm::vec3(0.f, 1.f, 0.f)
							),
							glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f) //!< Camera projection
						);
						camStr = std::string("Camera: Centre");
						m_currentCamPos++;
						break;
					case 3:
						Renderer3D::uploadCamera(
							Shader3D,
							glm::lookAt(					//!< Top-Right
								glm::vec3(2.f, 2.f, 0.f),
								glm::vec3(0.f, 0.f, -6.f),
								glm::vec3(0.f, 1.f, 0.f)
							),
							glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f) //!< Camera projection
						);
						camStr = std::string("Camera: Top-Right");
						m_currentCamPos = 0;
						break;
					}
				}
				m_spacePressed = true;
			}
			else m_spacePressed = false;

			if (InputPoller::isKeyPressed(NG_KEY_UP)) {
				if (!m_directionKeyPressed[0]) {
					LOG_INFO("UP");
					models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.5f));
				}
				m_directionKeyPressed[0] = true;
			}
			else m_directionKeyPressed[0] = false;
			if (InputPoller::isKeyPressed(NG_KEY_DOWN)) {
				if (!m_directionKeyPressed[1]) {
					LOG_INFO("DOWN");
					models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -5.5f));
				}
				m_directionKeyPressed[1] = true;
			}
			else m_directionKeyPressed[1] = false;
			if (InputPoller::isKeyPressed(NG_KEY_LEFT)) {
				if (!m_directionKeyPressed[2]) {
					LOG_INFO("LEFT");
					models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.f, -6.f));
				}
				m_directionKeyPressed[2] = true;
			}
			else m_directionKeyPressed[2] = false;
			if (InputPoller::isKeyPressed(NG_KEY_RIGHT)) {
				if (!m_directionKeyPressed[3]) {
					LOG_INFO("RIGHT");
					models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.f, -6.f));
				}
				m_directionKeyPressed[3] = true;
			}
			else m_directionKeyPressed[3] = false;
			if (m_directionKeyPressed[0] == false && m_directionKeyPressed[1] == false && m_directionKeyPressed[2] == false && m_directionKeyPressed[3] == false)
				models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
			

			/**\ Rendering the scene */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			Renderer3D::beginScene(); //!< Adds the depth testing

			Renderer3D::submit(pyramidVAO, pyramidMaterial, models[0]); 
			Renderer3D::submit(cubeVAO, letterCubeMaterial, models[1]);
			Renderer3D::submit(cubeVAO, numberCubeMaterial, models[2]);

			Renderer3D::endScene();


			Renderer2D::beginScene(true); //!< Removes the depth testing


			Renderer2D::submitQuad(
				Quad::create({50.f, 540.f }, { 15.f, 15.f }),
				gearTexture,
				{ 1.f, 1.f, 1.f, 1.f },
				m_badgeRotation+=3.f
			);

			Renderer2D::submitText(fpsStr.c_str(), { 100.f, 550.f }, { 0.f, 0.f, 0.f, 1.f });

			Renderer2D::submitText(camStr.c_str(), { 550.f, 550.f }, { 1.f, 0.f, 0.f, 1.f });

			Renderer2D::endScene();

			m_Window->onUpdate(elapsedTime);

			elapsedTime = timer::getFrameTime();

			fpsStr = std::string("fps: ") + std::to_string((int)(1 / elapsedTime));
			//LOG_INFO("fps: {0}", 1.f / elapsedTime);
		}
	}
}