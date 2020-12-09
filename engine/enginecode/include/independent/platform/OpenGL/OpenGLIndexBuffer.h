/** \file OpenGLIndexBuffer.h */
#pragma once
#include "rendering/indexBuffer.h"
namespace Engine
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t * indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();
		virtual inline uint32_t getID() const override { return m_OpenGL_ID; }
		virtual inline uint32_t getCount() const override { return m_count; }
	private:
		uint32_t m_OpenGL_ID;
		uint32_t m_count;
	};
}