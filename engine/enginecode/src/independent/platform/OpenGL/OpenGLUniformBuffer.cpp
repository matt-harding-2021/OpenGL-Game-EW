/**\ file OpenGLUniformBuffer.cpp */
#include "engine_pch.h"
#include "platform/OpenGL/OpenGLUniformBuffer.h"
#include <glad/glad.h>

namespace Engine {
	uint32_t OpenGLUniformBuffer::s_blockNumber = 0;
	OpenGLUniformBuffer::OpenGLUniformBuffer(const UniformBufferLayout& arg_layout)
	{
		m_UBLayout = arg_layout;
		m_blockNumber = s_blockNumber;
		s_blockNumber++;
		glGenBuffers(1, &m_OpenGL_ID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_OpenGL_ID);
		glBufferData(GL_UNIFORM_BUFFER, m_UBLayout.getStride(), nullptr, GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_blockNumber, m_OpenGL_ID, 0, m_UBLayout.getStride());

		/**\ Populating the Uniform Cache 
		*	 Will run through the Uniform Buffer Layout and work out the offset and size of each element
		*/
		for (auto& element : m_UBLayout)
		{
			m_uniformCache[element.m_name] = std::pair<uint32_t, uint32_t>(element.m_offset, element.m_size);
		}
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_OpenGL_ID);
	}
	/**\	Attaching to the shader
	*		blockIndex is the binding point in the shader
	*		blockName is the uniform block name in the shader glsl file
	*/
	void OpenGLUniformBuffer::attachShaderBlock(const std::shared_ptr<Shader>& arg_shader, const char* arg_blockName)
	{
		uint32_t blockIndex = glGetUniformBlockIndex(arg_shader->getID(), arg_blockName);
		glUniformBlockBinding(arg_shader->getID(), blockIndex, m_blockNumber);
	}
	void OpenGLUniformBuffer::uploadData(const char* arg_Name, void* arg_Data)
	{
		/**\ Allocates data sizes in the memory */
		auto& pair = m_uniformCache[arg_Name];
		glBufferSubData(GL_UNIFORM_BUFFER, pair.first, pair.second, arg_Data); //!< 
	}
}