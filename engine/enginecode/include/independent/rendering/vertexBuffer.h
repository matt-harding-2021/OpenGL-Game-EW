/**\ file vertexBuffer.h */
#pragma once

#include "rendering/bufferLayout.h"

namespace Engine {
	class VertexBuffer
	{
	public:
		static VertexBuffer* create(void* vertices, uint32_t size, BufferLayout layout);
		~VertexBuffer() = default;
		virtual void edit(void* vertices, uint32_t size, uint32_t offset) = 0;
		virtual inline uint32_t getRenderID() const = 0;
		virtual inline const BufferLayout& getLayout() const = 0;
	};
}