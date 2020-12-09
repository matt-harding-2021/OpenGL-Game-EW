/**\ file indexBuffer.h */

#pragma once
#include <cstdint>

namespace Engine {
	/**\ Class IndexBuffer
	*	 API agnostic index buffer 
	*/
	class IndexBuffer
	{
	public:
		static IndexBuffer* create(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer() = default;
		virtual inline uint32_t getID() const = 0;
		virtual	inline uint32_t getCount() const = 0;
	};
}