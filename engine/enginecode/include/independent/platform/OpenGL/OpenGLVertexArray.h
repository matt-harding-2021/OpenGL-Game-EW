/** \file OpenGLVertexArray.h */
#pragma once

#include <vector>
#include <memory>

#include "rendering/vertexArray.h"

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& arg_vertexBuffer) override;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& arg_indexBuffer) override;

		virtual inline uint32_t getID() const override { return m_OpenGL_ID; }
		virtual inline uint32_t getDrawCount() const override { if (m_indexBuffer) { return m_indexBuffer->getCount(); } else { return 0; } }
		virtual inline std::shared_ptr<IndexBuffer> getIndexBuffer() const override { return m_indexBuffer; }
	private:
		uint32_t m_OpenGL_ID; 
		uint32_t m_attributeIndex = 0;

		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
	};
}