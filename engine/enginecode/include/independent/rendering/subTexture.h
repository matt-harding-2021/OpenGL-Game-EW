/**\ file subTexture.h */
#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "texture.h"

namespace Engine {

	class SubTexture
	{
	public:
		SubTexture() {

		}
		SubTexture(const std::shared_ptr<Texture>& arg_Texture, const glm::vec2& arg_UVStart, const glm::vec2& arg_UVEnd) : m_texture(arg_Texture), m_UVStart(arg_UVStart), m_UVEnd(arg_UVEnd)
		{
			m_size = (m_UVEnd - m_UVStart) * m_texture->getSize();
		}
		~SubTexture() {

		}

		inline glm::vec2 getUVStart() { return m_UVStart; }
		inline glm::vec2 getUVEnd() { return m_UVEnd; }
		glm::vec2 getSize() { return m_size; }
		
		glm::vec2 transformUV(float arg_U, float arg_V) //!< Takes original co-ordinate and returns the co-ordinate in the atlas (re-scales)
		{
			return m_UVStart + ((m_UVEnd - m_UVStart) * glm::vec2(arg_U, arg_V)); //!< Linear Interpolation
		}
	private:
		std::shared_ptr<Texture> m_texture;
		glm::vec2 m_UVStart;
		glm::vec2 m_UVEnd;
		glm::vec2 m_size; 
	};
}