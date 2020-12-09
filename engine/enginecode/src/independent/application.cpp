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
		m_mousePosCurrent = glm::vec2(e.getPos().x, e.getPos().y);
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
	*		OpenGL setup 
	*		Event loop with input polling
	*		Per frame update functions 
	*/
	void Application::run()
	{

	
#pragma region TEXTURES
		/**	Implementing the abstracted OpenGL Textures
		*
		*/
		//NEED TO MAKE API AGNOSTIC
		std::shared_ptr<Texture> textureAtlas;
		textureAtlas.reset(Texture::create("assets/textures/letterAndNumberCube(Dark).png"));
		SubTexture letterTexture(textureAtlas, { 0.f, 0.f }, { 1.f, 0.5f });
		SubTexture numberTexture(textureAtlas, { 0.f, 0.5f }, { 1.f, 1.f });

		std::shared_ptr<Texture> pyramidTex;
		unsigned char PxlColour[4] = { 155, 0, 55, 255 };
		pyramidTex.reset(Texture::create(1, 1, 4, PxlColour));

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
		std::shared_ptr<IndexBuffer> cubeIBO; //!< Index BufferObject

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
		*	Using the Phong lighting model
		*	Each shader object reads a text file and compiles it line by line into the OpenGL library shaders
		*/

		/**\ API AGNOSTIC SHADER */
		std::shared_ptr<Shader> TPShader;
		TPShader.reset(Shader::create("./assets/shaders/texturedPhong.glsl"));
#pragma endregion 
#pragma region UBOS
		/**\ Camera UBO */
		std::shared_ptr<UniformBuffer> cameraUBO;

		/**\ Setting the type of data for the camera UBO */
		UniformBufferLayout cameraLayout = { {"u_view", ShaderDataType::Mat4}, {"u_projection", ShaderDataType::Mat4} };
		glm::mat4 cameraView = glm::lookAt( //!< Camera view
			glm::vec3(0.f, 0.f, 0.f),
			glm::vec3(0.f, 0.f, -1.f),
			glm::vec3(0.f, 1.f, 0.f)
		);
		glm::mat4 cameraProj = glm::perspective(glm::radians(45.f), 1024.f / 800.f, 0.1f, 100.f); //!< Camera projection

		/**\ Initializing the camera UBO using the data */
		cameraUBO.reset(UniformBuffer::create(cameraLayout));

		cameraUBO->uploadData("u_view", glm::value_ptr(cameraView)); //!< Uploading the view
		cameraUBO->uploadData("u_projection", glm::value_ptr(cameraProj)); //!< Uploading the projection

		cameraUBO->attachShaderBlock(TPShader, "b_camera"); //!< Updating the UBO with the shader and position of uniforms within the shader



		/**\ Light UBO */
		std::shared_ptr<UniformBuffer> lightsUBO;

		/**\ Setting the type of data for the lights UBO */
		UniformBufferLayout lightsLayout = { {"u_lightPos", ShaderDataType::Float3}, {"u_viewPos", ShaderDataType::Float3}, {"u_lightColour", ShaderDataType::Float3}, {"u_tint", ShaderDataType::Float3} };
		glm::vec3 lightPos(0.f, 0.f, 4.f);
		glm::vec3 lightView(0.f, 0.f, 0.f);
		glm::vec3 lightColour(1.f, 1.f, 1.f);
		glm::vec3 tintColour(1.f, 1.f, 1.f);

		/**\ Initializing the lights UBO using the data */
		lightsUBO.reset(UniformBuffer::create(lightsLayout));

		lightsUBO->uploadData("u_lightPos", glm::value_ptr(lightPos));		 //!< Uploading the position
		lightsUBO->uploadData("u_viewPos", glm::value_ptr(lightView));		 //!< Uploading the view position
		lightsUBO->uploadData("u_lightColour", glm::value_ptr(lightColour)); //!< Uploading the colour
		lightsUBO->uploadData("u_tint", glm::value_ptr(tintColour));		 //!< Uploading the tint

		lightsUBO->attachShaderBlock(TPShader, "b_lights"); //!< Updating the UBO with the shader and position of uniforms within the shader
#pragma endregion 


		/**\ Setting the model transforms */
		glm::mat4 models[3];
		models[0] = glm::translate(glm::mat4(1.0f), glm::vec3(-2.f, 0.f, -6.f));
		models[1] = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -6.f));
		models[2] = glm::translate(glm::mat4(1.0f), glm::vec3(2.f, 0.f, -6.f));

		glEnable(GL_DEPTH_TEST);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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



			/*	Save as 0,0,0
			*	Each frame add local rotation to world rotation
			*	
			*	Convert mouse x and y to the cube local rotation with regard to world rotation
			*/
			if (InputPoller::isMouseButtonPressed(NG_MOUSE_BUTTON_1)) {
				//LOG_INFO("MouseCurrent: {0}, {1}", m_mousePosCurrent.x, m_mousePosCurrent.y);
				//LOG_INFO("MouseStart: {0}, {1}", m_mousePosStart.x, m_mousePosStart.y);
				//LOG_INFO("MouseDelta: {0}, {1}",  m_mousePosCurrent.x - m_mousePosStart.x, m_mousePosCurrent.y - m_mousePosStart.y);
				glm::vec2 mouseDelta = m_mousePosCurrent - m_mousePosStart;
				if (mouseDelta.x != 0.f)
					for (auto& model : models) { model = glm::rotate(model, elapsedTime * mouseDelta.x, glm::vec3(0.f, 1.f, 0.f)); }
				if (mouseDelta.y != 0.f)
					for (auto& model : models) { model = glm::rotate(model, elapsedTime * mouseDelta.y, glm::vec3(1.f, 0.f, 0.f)); }
			}
			else for (auto& model : models) { model = glm::rotate(model, elapsedTime, glm::vec3(0.f, 1.0, 0.f)); }

			m_mousePosStart = m_mousePosCurrent;
			

			

			// RENDERING //
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(TPShader->getID());




			// PYRAMID BINDING //
			glBindVertexArray(pyramidVAO->getID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidIBO->getID());
			glBindTexture(GL_TEXTURE_2D, pyramidTex->getID());

			// PYRAMID 1 RENDERING //
			GLuint uniformLocation;
			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_model"); 
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[0])); // Must include <glm/gtc/type_ptr.hpp>

			glDrawElements(GL_TRIANGLES, 3 * 6, GL_UNSIGNED_INT, nullptr);




			// CUBE BINDING //
			glBindVertexArray(cubeVAO->getID());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIBO->getID());
			glBindTexture(GL_TEXTURE_2D, textureAtlas->getID());
			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_texData"); // Can't replace with UBO because it is a sampler
			glUniform1i(uniformLocation, 0);


			// CUBE 1 RENDERING //
			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_model"); // Possibly replace other Uniforms with Uniform buffer objects
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[1]));

			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
			

			// CUBE 2 RENDERING //
			uniformLocation = glGetUniformLocation(TPShader->getID(), "u_model");
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(models[2]));

			glDrawElements(GL_TRIANGLES, cubeVAO->getDrawCount(), GL_UNSIGNED_INT, nullptr);
			







			m_Window->onUpdate(elapsedTime);
			elapsedTime = timer::getFrameTime();
		}
	}
}
