/**\ file OpenGLTexture.h */
#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "rendering/texture.h"
namespace Engine {
	/** Class OpenGLTexture
	*
	*	Abstracted class for handling OpenGL textures.
	*
	*	Can generate a texture using a file, or taking the parameters direct.
	*	Can further edit the textures offset (start position of the texture), size, and data.
	*/
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const char* arg_file); //!< Constructor that takes the texture filepath
		OpenGLTexture(uint32_t arg_width, uint32_t arg_height, uint32_t arg_channels, unsigned char* arg_data);
		~OpenGLTexture();

		virtual inline uint32_t getID() override { return m_OpenGL_ID; }
		virtual glm::vec2 getSize() override { return m_size; }
		virtual inline uint32_t getChannels() override { return m_channels; }
		virtual void edit(glm::vec2 arg_offset, glm::vec2 arg_size, uint32_t arg_channels, unsigned char* arg_data) override;
	private:
		uint32_t m_OpenGL_ID;
		glm::vec2 m_size;

		/**\ Channels of colour
		*	 Supporting 1, 3, and 4 channels:
		*	1 : r
		*	3 : r g b
		*	4 : r g b a
		*/
		uint32_t m_channels;

		void init(uint32_t arg_width, uint32_t arg_height, uint32_t arg_channels, unsigned char* arg_data);
	};
}