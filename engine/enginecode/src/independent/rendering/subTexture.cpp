/**\ file subTexture.cpp */
#include "engine_pch.h"
#include "rendering/subTexture.h"

namespace Engine {

	SubTexture::SubTexture()
	{
	}
	SubTexture::SubTexture(const std::shared_ptr<Texture>& arg_Texture, const glm::vec2& arg_UVStart, const glm::vec2& arg_UVEnd) : m_texture(arg_Texture), m_UVStart(arg_UVStart), m_UVEnd(arg_UVEnd)
	{
		m_size = (m_UVEnd - m_UVStart) * m_texture->getSize();
	}

	SubTexture::~SubTexture()
	{
	}

	/**\ Linear interpolation */
	glm::vec2 SubTexture::transformUV(float arg_U, float arg_V)
	{
		return m_UVStart + ((m_UVEnd - m_UVStart) * glm::vec2(arg_U, arg_V));
	}
}