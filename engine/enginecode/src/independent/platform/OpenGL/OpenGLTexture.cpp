/**\ file OpenGLTexture.cpp */
#include "engine_pch.h"
#include "platform/OpenGL/OpenGLTexture.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "systems/logging.h"
namespace Engine {
	/** Constructor (Argument: filepath)
	*	Loads the width, height, and channels from the file.
	*	If successful, calls init passing these variables.
	*/
	OpenGLTexture::OpenGLTexture(const char* arg_file)
	{
		int width, height, channels;
		unsigned char* data = stbi_load(arg_file, &width, &height, &channels, 0); //!< Loading the image from the filepath

		if (data) { init(width, height, channels, data); }

		stbi_image_free(data); 
	}
	/** Constructor (Argument: width, height, channels, data)
	*	If data argument exists, calls init, passing the arguments as they are.
	*/
	OpenGLTexture::OpenGLTexture(uint32_t arg_width, uint32_t arg_height, uint32_t arg_channels, unsigned char* arg_data)
	{
		if (arg_data) { init(arg_width, arg_height, arg_channels, arg_data); }
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_OpenGL_ID);
	}

	void OpenGLTexture::init(uint32_t arg_width, uint32_t arg_height, uint32_t arg_channels, unsigned char* arg_data)
	{
		glGenTextures(1, &m_OpenGL_ID); //!< Generates the texture
		glBindTexture(GL_TEXTURE_2D, m_OpenGL_ID); //!< Binds the texture

		/**\ Setting up the texture parameters */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (arg_channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, arg_width, arg_height, 0, GL_RGB, GL_UNSIGNED_BYTE, arg_data);
		else if (arg_channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, arg_width, arg_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, arg_data);
		else return;
		glGenerateMipmap(GL_TEXTURE_2D);

		m_size = glm::vec2(arg_width, arg_height);
		m_channels = arg_channels;
	}

	void OpenGLTexture::edit(glm::vec2 arg_offset, glm::vec2 arg_size, uint32_t arg_channels, unsigned char* arg_data)
	{
		glBindTexture(GL_TEXTURE_2D, m_OpenGL_ID);
		if (arg_data && arg_channels == m_channels) {
			if (m_channels == 3) glTextureSubImage2D(GL_TEXTURE_2D, 0, arg_offset.x, arg_offset.y, arg_size.x, arg_size.y, GL_RGB, GL_UNSIGNED_BYTE, arg_data);
			else if (m_channels == 4) glTextureSubImage2D(GL_TEXTURE_2D, 0, arg_offset.x, arg_offset.y, arg_size.x, arg_size.y, GL_RGBA, GL_UNSIGNED_BYTE, arg_data);
		} 
		else {
			LOG_ERROR("OpenGLTexture::edit() error,  data:{0}  channels{1}", arg_data, arg_channels);
		}
	}
}