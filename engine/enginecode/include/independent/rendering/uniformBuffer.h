/**\ file uniformBuffer.h */
#pragma once

#include <unordered_map>
#include <memory>

#include "bufferLayout.h"
#include "shader.h"
namespace Engine {
	class UniformBuffer
	{
	public:
		static UniformBuffer* create(const UniformBufferLayout& arg_layout);
		virtual ~UniformBuffer() = default;

		virtual void attachShaderBlock(const std::shared_ptr<Shader>& arg_shader, const char* arg_blockName) = 0;
		virtual void uploadData(const char* arg_Name, void* arg_Data) = 0;

		virtual uint32_t getRendererID() = 0;
		virtual UniformBufferLayout getUBOLayout() = 0;
	protected:
		UniformBufferLayout m_UBLayout;
		std::unordered_map<const char*, std::pair<uint32_t, uint32_t>> m_uniformCache; //!< Maps from name to its offset in size;
		uint32_t m_blockNumber;  //<! Binding point on the GPU for the uniform block
	};
}