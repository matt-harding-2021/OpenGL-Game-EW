/**\ file vertexArray.h */

#pragma once

#include <vector>
#include <memory>
#include "vertexBuffer.h"
#include "indexBuffer.h"

namespace Engine {
	/**\ Class IndexBuffer
	*	 API agnostic index buffer
	*/
	class VertexArray
	{
	public:
		static VertexArray* create();
		virtual ~VertexArray() = default;

		virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual inline uint32_t getID() const = 0;
		virtual inline uint32_t getDrawCount() const = 0;
	};
}