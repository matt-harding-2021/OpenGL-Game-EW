/**\ file OpenGLUniformBuffer.h */
#pragma once

#include "rendering/uniformBuffer.h"

namespace Engine {
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(const UniformBufferLayout& arg_layout); //!< Constructor takes a UniformBufferLayout object as argument
		~OpenGLUniformBuffer();

		void attachShaderBlock(const std::shared_ptr<Shader>& arg_shader, const char* arg_blockName) override; //Const std::sharedptr?
		void uploadData(const char* arg_Name, void* arg_Data) override;

		inline uint32_t getRendererID() override { return m_OpenGL_ID; }
		inline UniformBufferLayout getUBOLayout() override { return m_UBLayout; }
	private:
		uint32_t m_OpenGL_ID; //!< OpenGL ID
		static uint32_t s_blockNumber;  //<! Beginning binding point on the GPU
	};
}