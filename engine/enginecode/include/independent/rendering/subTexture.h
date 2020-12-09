/**\ file subTexture.h */
#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "texture.h"

namespace Engine {

	class SubTexture
	{
	public:
		SubTexture();
		SubTexture(const std::shared_ptr<Texture>& arg_Texture, const glm::vec2& arg_UVStart, const glm::vec2& arg_UVEnd);
		~SubTexture();

		inline glm::vec2 getUVStart() { return m_UVStart; }
		inline glm::vec2 getUVEnd() { return m_UVEnd; }
		glm::vec2 getSize() { return m_size; }
		//glm::vec2 getSizef() { return { static_cast<float>(m_size.x), static_cast<float>(m_size.y) }; } //!< Avoids type coercion possibilities by casting to a float already

		glm::vec2 transformUV(float arg_U, float arg_V); //!< Takes original co-ordinate and returns the co-ordinate in the atlas (re-scales)
	private:
		std::shared_ptr<Texture> m_texture;
		glm::vec2 m_UVStart;
		glm::vec2 m_UVEnd;
		glm::vec2 m_size; 
	};
}