/** \file OpenGLVertexBuffer.cpp */

#include "engine_pch.h"
#include <glad/glad.h>
#include "platform/OpenGL/OpenGLVertexBuffer.h"

namespace Engine
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* arg_vertices, uint32_t arg_size, const VertexBufferLayout& arg_layout) : m_layout(arg_layout)
	{
		glCreateBuffers(1, &m_OpenGL_ID);
		glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
		glBufferData(GL_ARRAY_BUFFER, arg_size, arg_vertices, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_OpenGL_ID);
	}
	void OpenGLVertexBuffer::edit(void* arg_vertices, uint32_t arg_size, uint32_t arg_offset)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_OpenGL_ID);
		glBufferSubData(GL_ARRAY_BUFFER, arg_offset, arg_size, arg_vertices);
	}
}