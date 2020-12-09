/**\ file renderAPI.cpp */
#include "engine_pch.h"
#include "rendering/renderAPI.h"
#include "rendering/uniformBuffer.h"
#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/shader.h"
#include "rendering/texture.h"

#include "platform/OpenGL/OpenGLUniformBuffer.h"
#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "platform/OpenGL/OpenGLTexture.h"

#include "systems/logging.h"
namespace Engine {
	RenderAPI::API RenderAPI::s_currentAPI = RenderAPI::API::OpenGL;

	Texture* Texture::create(const char* arg_file)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(arg_file);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}

	Texture* Texture::create(uint32_t arg_width, uint32_t arg_height, uint32_t arg_channels, unsigned char* arg_data)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLTexture(arg_width, arg_height, arg_channels, arg_data);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}

	UniformBuffer* UniformBuffer::create(const UniformBufferLayout& arg_layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLUniformBuffer(arg_layout);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}

	IndexBuffer* IndexBuffer::create(uint32_t* arg_indices, uint32_t arg_count)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(arg_indices, arg_count);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}

	VertexBuffer* VertexBuffer::create(void* arg_vertices, uint32_t arg_size, const VertexBufferLayout& arg_layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(arg_vertices, arg_size, arg_layout);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}

	VertexArray* VertexArray::create()
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}
	/**\ API AGNOSTIC SHADER */
	Shader* Shader::create(const char* arg_VerFilepath, const char* arg_FragFilepath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(arg_VerFilepath, arg_FragFilepath); 
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}
	Shader* Shader::create(const char* arg_Filepath)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLShader(arg_Filepath);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}
}