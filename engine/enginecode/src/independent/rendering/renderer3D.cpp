/**\ file renderer3D.cpp */

#include "engine_pch.h"
#include "rendering/renderer3D.h"

#include <glad/glad.h>

namespace Engine {
	std::shared_ptr<Renderer3D::InternalData> Renderer3D::s_data = nullptr;
	// UBOs in init()
	void Renderer3D::init()
	{
		s_data.reset(new InternalData);

		unsigned char PxlColour[4] = { 155, 0, 55, 255 };
		s_data->texture.reset(Texture::create(1, 1, 4, PxlColour));
		s_data->tint = { 1.f, 1.f, 1.f, 1.f };
	}
	void Renderer3D::beginScene(const SceneWideUniforms& arg_uniforms)
	{
		s_data->uniforms = arg_uniforms;
	}

	//Could be API agnostic, put bind commands in abstracted buffers
	void Renderer3D::submit(const std::shared_ptr<VertexArray>& arg_geometry, const std::shared_ptr<Material> arg_material, const glm::mat4& arg_model)
	{
		//Bind shader
		glUseProgram(arg_material->getShader()->getID());
		
		//Apply Uniforms

		//Bind geometry (VAO and IBO)

		//Submit draw call
	}
	void Renderer3D::endScene()
	{
		s_data->uniforms.clear();
	}
}