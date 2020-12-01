/**\ file renderAPI.cpp */
#include "engine_pch.h"
#include "rendering/renderAPI.h"
#include "rendering/indexBuffer.h"
#include "rendering/vertexBuffer.h"
#include "rendering/vertexArray.h"
#include "rendering/shader.h"

#include "platform/OpenGL/OpenGLIndexBuffer.h"
#include "platform/OpenGL/OpenGLVertexBuffer.h"
#include "platform/OpenGL/OpenGLVertexArray.h"
#include "platform/OpenGL/OpenGLShader.h"

#include "systems/logging.h"
namespace Engine {
	RenderAPI::API RenderAPI::s_currentAPI = RenderAPI::API::OpenGL;

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
			break;
		case RenderAPI::API::Direct3d:
			LOG_ERROR("Direct3d rendering API: Not supported");
			break;
		case RenderAPI::API::Vulkan:
			LOG_ERROR("Vulkan rendering API: Not supported");
			break;
		}
	}

	VertexBuffer* VertexBuffer::create(void* vertices, uint32_t size, BufferLayout layout)
	{
		switch (RenderAPI::getAPI())
		{
		case RenderAPI::API::None:
			LOG_ERROR("No rendering API: Not supported");
			break;
		case RenderAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size, layout);
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
	/*	API AGNOSTIC SHADER
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
	*/
}