/** \file application.cpp */
#include "engine_pch.h"

#include <glad/glad.h>

#ifdef NG_PLATFORM_WINDOWS
#include "platform/windows/GLFWWindowsSystem.h"
#include "platform/windows/GLFWWindowImpl.h"
#endif

/* stb_image now in OpenGLTexture.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
*/

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/application.h"

#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/shader.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLTexture.h"

#include "rendering/subTexture.h"

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
		
		//!< Key events
		dispatcher.dispatch<KeyPressedEvent>(std::bind(&Application::onKeyPressed, this, std::placeholders::_1));
		dispatcher.dispatch<KeyReleasedEvent>(std::bind(&Application::onKeyReleased, this, std::placeholders::_1));

		//!< Mouse events
		dispatcher.dispatch<MouseButtonPressedEvent>(std::bind(&Application::onMouseButtonPressed, this, std::placeholders::_1));
		dispatcher.dispatch<MouseButtonReleasedEvent>(std::bind(&Application::onMouseButtonReleased, this, std::placeholders::_1));
		dispatcher.dispatch<MouseMovedEvent>(std::bind(&Application::onMouseMoved, this, std::placeholders::_1));
		dispatcher.dispatch<MouseScrolledEvent>(std::bind(&Application::onMouseScrolled, this, std::placeholders::_1));

		//!< Window events
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
		dispatcher.dispatch<WindowFocusEvent>(std::bind(&Application::onWindowFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowLostFocusEvent>(std::bind(&Application::onWindowLostFocus, this, std::placeholders::_1));
		dispatcher.dispatch<WindowMovedEvent>(std::bind(&Application::onWindowMoved, this, std::placeholders::_1));
		dispatcher.dispatch<WindowResizeEvent>(std::bind(&Application::onWindowResize, this, std::placeholders::_1));
		
	}
	/**\ Functions the dispatcher calls depending on event type */
	bool Application::onKeyPressed(KeyPressedEvent& e) { 
		if (e.GetRepeatCount() == 0) LOG_INFO("Key Pressed: {0}", e.GetKeyCode());
		else if (e.GetRepeatCount() == 1) LOG_INFO("Key Repeated: {0}", e.GetKeyCode());
		return true; 
	}
	bool Application::onKeyReleased(KeyReleasedEvent& e) { 
		LOG_INFO("Key Released: {0}", e.GetKeyCode());
		return true; 
	}
	bool Application::onKeyTyped(KeyTypedEvent& e) { return true; } 
	bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) { 
		LOG_INFO("Mouse Button Pressed: {0}", e.getButton());
		return true;
	}
	bool Application::onMouseButtonReleased(MouseButtonReleasedEvent& e) {
		LOG_INFO("Mouse Button Released: {0}", e.getButton());
		return true;
	}
	bool Application::onMouseMoved(MouseMovedEvent& e) { 
		LOG_INFO("Mouse Moved: {0}x{1}", e.getPos().x, e.getPos().y);
		return true; 
	}
	bool Application::onMouseScrolled(MouseScrolledEvent& e) { 
		LOG_INFO("Mouse Scrolled: {0}x{1}", e.getXOffset(), e.getYOffset());
		return true; 
	}
	bool Application::onWindowClose(WindowCloseEvent& e) {
		LOG_INFO("Window Closed");
		m_Running = false;  //!< Set the application loop boolean to false
		return true;
	}
	bool Application::onWindowFocus(WindowFocusEvent& e) { 
		LOG_INFO("Window Focused");
		return true; 
	}
	bool Application::onWindowLostFocus(WindowLostFocusEvent& e) { 
		LOG_INFO("Window Lost Focus");
		return true; 
	}
	bool Application::onWindowMoved(WindowMovedEvent& e) { 
		LOG_INFO("Window Moved: {0}x{1}", e.getPos().x, e.getPos().y);
		return true; 
	}
	bool Application::onWindowResize(WindowResizeEvent& e) {
		LOG_INFO("Window Resized: {0}x{1}", e.getWidth(), e.getHeight());
		return true;
	}


	/**	Once the app is initialised in entrypoint.h, app.run() begins the actual program. Contains: 
	*		OpenGL setup 
	*		Event loop with input polling
	*		Per frame update functions 
	*/
	void Application::run()
	{

	/**	Implemnting the abstracted OpenGL Textures
	*
	*/
#pragma region TEXTURES

		std::shared_ptr<OpenGLTexture> textureAtlas;
		//letterTexture.reset(new OpenGLTexture("assets/textures/letterCube.png"));
		textureAtlas.reset(new OpenGLTexture("assets/textures/letterAndNumberCube.png"));

		/*
		std::shared_ptr<OpenGLTexture> numberTexture;
		numberTexture.reset(new OpenGLTexture("assets/textures/numberCube.png"));
		*/

		SubTexture letterTexture(textureAtlas, { 0.f, 0.f }, { 1.f, 0.5f });
		SubTexture numberTexture(textureAtlas, { 0.f, 0.5f }, { 1.f, 1.f });

#pragma endregion


#pragma region RAW_DATA
		float cubeVertices[8 * 24] = {
			//	 <------ Pos ------>  <--- normal --->  <-- UV -->
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,	letterTexture.transformUV(0.f,   0.f).x,	letterTexture.transformUV(0.f,   0.f).y,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,	letterTexture.transformUV(0.f,   0.5f).x,	letterTexture.transformUV(0.f,   0.5f).y,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,	letterTexture.transformUV(0.33f, 0.5f).x,	letterTexture.transformUV(0.33f, 0.5f).y,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,	letterTexture.transformUV(0.33f, 0.f).x,	letterTexture.transformUV(0.33f, 0.f).y,
				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,	letterTexture.transformUV(0.33f, 0.5f).x,	letterTexture.transformUV(0.33f, 0.5f).y,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,	letterTexture.transformUV(0.66f, 0.5f).x,	letterTexture.transformUV(0.66f, 0.5f).y,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,	letterTexture.transformUV(0.66f, 0.f).x,	letterTexture.transformUV(0.66f, 0.f).y,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,	letterTexture.transformUV(0.33,  0.f).x,	letterTexture.transformUV(0.33,  0.f).y
																										 
				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,	letterTexture.transformUV(1.f,   0.f).x,	letterTexture.transformUV(1.f,   0.f).y,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,	letterTexture.transformUV(0.66f, 0.f).x,	letterTexture.transformUV(0.66f, 0.f).y,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,	letterTexture.transformUV(0.66f, 0.5f).x,	letterTexture.transformUV(0.66f, 0.5f).y,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,	letterTexture.transformUV(1.0f,  0.5f).x,	letterTexture.transformUV(1.0f,  0.5f).y,
																																		   
				 0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	letterTexture.transformUV(0.f,   0.5f).x,	letterTexture.transformUV(0.f,   0.5f).y,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,	letterTexture.transformUV(0.f,   1.0f).x,	letterTexture.transformUV(0.f,   1.0f).y,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,	letterTexture.transformUV(0.33f, 1.0f).x,	letterTexture.transformUV(0.33f, 1.0f).y,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,	letterTexture.transformUV(0.3f,  0.5f).x,	letterTexture.transformUV(0.3f,  0.5f).y,
																																		   
				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,	letterTexture.transformUV(0.66f, 0.5f).x,	letterTexture.transformUV(0.66f, 0.5f).y,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,	letterTexture.transformUV(0.33f, 0.5f).x,	letterTexture.transformUV(0.33f, 0.5f).y,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,	letterTexture.transformUV(0.33f, 1.0f).x,	letterTexture.transformUV(0.33f, 1.0f).y,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,	letterTexture.transformUV(0.66f, 1.0f).x,	letterTexture.transformUV(0.66f, 1.0f).y,
																																		   
				 0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,	letterTexture.transformUV(1.0f,  1.0f).x,	letterTexture.transformUV(1.0f,  1.0f).y,
				 0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,	letterTexture.transformUV(1.0f,  0.5f).x,	letterTexture.transformUV(1.0f,  0.5f).y,
				 0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,	letterTexture.transformUV(0.66f, 0.5f).x,	letterTexture.transformUV(0.66f, 0.5f).y,
				 0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,	letterTexture.transformUV(0.66f, 1.0f).x,	letterTexture.transformUV(0.66f, 1.0f).y
		};
		/*
		float cubeVertices[8 * 24] = {
			//	 <------ Pos ------>  <--- normal --->  <-- UV -->
				 0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,   0.f,
				 0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.f,   0.5f,
				-0.5f, -0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f, 0.5f,
				-0.5f,  0.5f, -0.5f,  0.f,  0.f, -1.f,  0.33f, 0.f,

				-0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33f, 0.5f,
				 0.5f, -0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f, 0.5f,
				 0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.66f, 0.f,
				-0.5f,  0.5f, 0.5f,   0.f,  0.f,  1.f,  0.33,  0.f,

				-0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  1.f,   0.f,
				 0.5f, -0.5f, -0.5f,  0.f, -1.f,  0.f,  0.66f, 0.f,
				 0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  0.66f, 0.5f,
				-0.5f, -0.5f, 0.5f,   0.f, -1.f,  0.f,  1.0f,  0.5f,

				 0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.f,   0.5f,
				 0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.f,   1.0f,
				-0.5f,  0.5f, -0.5f,  0.f,  1.f,  0.f,  0.33f, 1.0f,
				-0.5f,  0.5f, 0.5f,   0.f,  1.f,  0.f,  0.3f,  0.5f,

				-0.5f,  0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f, 0.5f,
				-0.5f,  0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f, 0.5f,
				-0.5f, -0.5f, -0.5f, -1.f,  0.f,  0.f,  0.33f, 1.0f,
				-0.5f, -0.5f, 0.5f,  -1.f,  0.f,  0.f,  0.66f, 1.0f,

				 0.5f, -0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  1.0f,
				 0.5f,  0.5f, -0.5f,  1.f,  0.f,  0.f,  1.0f,  0.5f,
				 0.5f,  0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 0.5f,
				 0.5f, -0.5f, 0.5f,   1.f,  0.f,  0.f,  0.66f, 1.0f
		};
		*/
		float pyramidVertices[6 * 16] = {
			//	 <------ Pos ------>  <--- colour ---> 
				-0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f, //  square Magneta
				 0.5f, -0.5f, -0.5f,  0.8f, 0.2f, 0.8f,
				 0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,
				-0.5f, -0.5f,  0.5f,  0.8f, 0.2f, 0.8f,

				-0.5f, -0.5f, -0.5f,  0.2f, 0.8f, 0.2f,  //triangle Green
				-0.5f, -0.5f,  0.5f,  0.2f, 0.8f, 0.2f,
				 0.0f,  0.5f,  0.0f,  0.2f, 0.8f, 0.2f,

				-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f, //triangle Red
				 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.f,
				 0.0f,  0.5f,  0.0f,  1.0f, 0.0f, 0.f,

				 0.5f, -0.5f,  0.5f,  0.8f, 0.8f, 0.2f, //  triangle Yellow
				 0.5f, -0.5f, -0.5f,  0.8f, 0.8f, 0.2f,
				 0.0f,  0.5f,  0.0f,  0.8f, 0.8f, 0.2f,

				 0.5f, -0.5f, -0.5f,  0.f, 0.2f, 1.0f,//  triangle Blue
				-0.5f, -0.5f, -0.5f,  0.f, 0.2f, 1.0f,
				 0.0f,  0.5f,  0.0f,  0.f, 0.2f, 1.0f
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

		/**	Implementing the abstracted frame buffers for OpenGL
		*	Doesn't include any actual openGL library calls.
		*	This means that other rendering libraries could be used in place of openGL in the future.
		*
		*	Rendering buffers allocate memory in the GPU that will be used to hold data about rendering objects (in this instance, a cube and a pyramid).
		*/
#pragma region GL_BUFFERS
		/**\ Creates new abstracted buffer objects for a cube */
		std::shared_ptr<VertexArray> cubeVAO; //!< Vertex Array Object,
		std::shared_ptr<VertexBuffer> cubeVBO; //!< Vertex Buffer Object,
		std::shared_ptr<IndexBuffer> cubeIBO; //!< Index BufferObject

		cubeVAO.reset(VertexArray::create());

		BufferLayout cubeBL = { ShaderDataType::Float3, ShaderDataType::Float3, ShaderDataType::Float2 };
		cubeVBO.reset(VertexBuffer::create(cubeVertices, sizeof(cubeVertices), cubeBL));

		cubeIBO.reset(IndexBuffer::create(cubeIndices, 36));

		cubeVAO->addVertexBuffer(cubeVBO);
		cubeVAO->setIndexBuffer(cubeIBO);

		
		std::shared_ptr<VertexArray> pyramidVAO;
		std::shared_ptr<VertexBuffer> pyramidVBO;
		std::shared_ptr<IndexBuffer> pyramidIBO;

		pyramidVAO.reset(VertexArray::create());

		BufferLayout pyramidBL = { ShaderDataType::Float3, ShaderDataType::Float3 };
		pyramidVBO.reset(VertexBuffer::create(pyramidVertices, sizeof(pyramidVertices), pyramidBL));

		pyramidIBO.reset(IndexBuffer::create(pyramidIndices, 18));

		pyramidVAO->addVertexBuffer(pyramidVBO);
		pyramidVAO->setIndexBuffer(pyramidIBO);
#pragma endregion

		/**	Implemnting the abstracted OpenGL Shaders
		*	Using the Phong lighting model
		*	Each shader object reads a text file and compiles it line by line into the OpenGL library shaders
		*/
#pragma region SHADERS

		/*	API AGNOSTIC SHADER
			CHANGE FILES: 
		*		rederAPI.cpp
		*		shader.h
		*		OpenGLShader.h

		std::shared_ptr<Shader> FCShader;
		FCShader.reset(Shader::create("./assets/shaders/flatColour.glsl"));

		std::shared_ptr<Shader> TPShader;
		TPShader.reset(Shader::create("./assets/shaders/texturedPhong.glsl"));
		*/
		std::shared_ptr<OpenGLShader> FCShader;
		FCShader.reset(new OpenGLShader("./assets/shaders/flatColour.glsl"));

		std::shared_ptr<OpenGLShader> TPShader;
		TPShader.reset(new OpenGLShader("./assets/shaders/texturedPhong.glsl"));

#pragma endregion 

		glm::mat4 view = glm::lookAt(
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.f, 1.f, 0.f)
		);
		glm::mat4 projection = glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f);

		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.f, -6.f));

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		float elapsedTime = 0;

		/**	The main event loop for the application. Contains:
		*	Event polling
		*	Update functions
		*/
		while (m_Running) {

			timer::startFrameTimer();
			//LOG_INFO("fps: {0}", 1.f / timer::getFrameTime());
			float totalTimeElapsed = timer::getMarkerTimer();
			//LOG_INFO("totalTimeElapsed: {0}", totalTimeElapsed);


			//if (InputPoller::isKeyPressed(NG_KEY_W)) LOG_INFO("Input Poller: W Pressed");
			//if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1)) LOG_ERROR("Mouse Button 1 Pressed");
			//LOG_INFO("Mouse Pos: {0} x {1}", InputPoller::getMousePosition().x, InputPoller::getMousePosition().y);





			for (auto& model : models) { model = glm::rotate(model, elapsedTime, glm::vec3(0.f, 1.0, 0.f)); }

			// Do frame stuff
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(FCShader->getID());
			glBindVertexArray(pyramidVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO->getRenderID());

			GLuint uniformLocation;

			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[0])); // Must include <glm/gtc/type_ptr.hpp>

			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_view");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));

			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_projection");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

			/*
			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_lightColour");
			glUniform3f(uniformLocation, 1.f, 1.f, 1.f);

			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_lightPos");
			glUniform3f(uniformLocation, 1.f, 4.f, 6.f);

			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_viewPos");
			glUniform3f(uniformLocation, 0.f, 0.f, 0.f);

			glBindTexture(GL_TEXTURE_2D, letterTexture);
			uniformLocation = glGetUniformLocation(FCShader->getID(), "u_texData");
			glUniform1i(uniformLocation, 0);
			*/

			glDrawElements(GL_TRIANGLES, 3 * 6, GL_UNSIGNED_INT, nullptr);

			



			glUseProgram(TPShader->getID());
			
			glBindVertexArray(cubeVAO->getRenderID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO->getRenderID());


			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[1]));

			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_view");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));

			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_projection");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));

			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_lightColour");
			glUniform3f(uniformLocation, 1.f, 1.f, 1.f);

			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_lightPos");
			glUniform3f(uniformLocation, 1.f, 4.f, 6.f);

			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_viewPos");
			glUniform3f(uniformLocation, 0.f, 0.f, 0.f);

			glBindTexture(GL_TEXTURE_2D, textureAtlas->getID());
			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_texData");
			glUniform1i(uniformLocation, 0);

			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);

			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[2]));

			//glBindTexture(GL_TEXTURE_2D, numberTexture->getID());

			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
			



			m_Window->onUpdate(elapsedTime);
			elapsedTime = timer::getFrameTime();
		}
	}
}
