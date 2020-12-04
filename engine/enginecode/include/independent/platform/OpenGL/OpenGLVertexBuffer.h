/** \file OpenGLVertexBuffer.h */
#pragma once

#include "rendering/vertexBuffer.h"
namespace Engine 
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		//Comment these
		OpenGLVertexBuffer(void* arg_vertices, uint32_t arg_size, const VertexBufferLayout& arg_layout);
		virtual ~OpenGLVertexBuffer();
		virtual void edit(void* arg_vertices, uint32_t arg_size, uint32_t arg_offset) override;
		virtual inline uint32_t getRenderID() const override { return m_OpenGL_ID; }
		virtual inline const VertexBufferLayout& getLayout() const override { return m_layout; }
	private:
		uint32_t m_OpenGL_ID; //!< Render ID
		VertexBufferLayout m_layout;
	};
}