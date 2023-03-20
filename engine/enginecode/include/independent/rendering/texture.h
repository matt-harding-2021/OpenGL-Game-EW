/**\ file texture.h */
#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Engine {
	/**\ API AGNOSTIC TEXTURE CLASS */
	class Texture
	{
	public:
		static Texture* create(const char* arg_file);
		static Texture* create(uint32_t arg_width, uint32_t arg_height, uint32_t arg_channels, unsigned char* arg_data);
		~Texture() = default;

		virtual inline uint32_t getID() = 0;
		virtual glm::vec2 getSize() = 0;
		virtual inline uint32_t getChannels() = 0;
		virtual void edit(glm::vec2 arg_offset, glm::vec2 arg_size, uint32_t arg_channels, unsigned char* arg_data) = 0;
	private:
		uint32_t m_OpenGL_ID;
		glm::vec2 m_size;
		uint32_t m_channels;
	};
}