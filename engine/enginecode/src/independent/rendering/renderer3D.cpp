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
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, s_data->PxlColour));
		s_data->defaultTint = { 1.f, 1.f, 1.f, 1.f };
	}
	void Renderer3D::uploadCamera(const std::shared_ptr<Shader> arg_shader, glm::mat4 arg_view, glm::mat4 arg_projection) {
		s_data->cameraUBO.reset(UniformBuffer::create(s_data->cameraLayout));
		s_data->cameraUBO->attachShaderBlock(arg_shader, "b_camera"); //!< Updating the camera UBO with the position of camera uniforms within the shader

		s_data->cameraUBO->uploadData("u_view", glm::value_ptr(arg_view));
		s_data->cameraUBO->uploadData("u_projection", glm::value_ptr(arg_projection));
	}
	void Renderer3D::uploadLights(const std::shared_ptr<Shader> arg_shader, glm::vec3 arg_position, glm::vec3 arg_view, glm::vec3 arg_colour, glm::vec4 arg_tint) {
		s_data->lightsUBO.reset(UniformBuffer::create(s_data->lightsLayout));
		s_data->lightsUBO->attachShaderBlock(arg_shader, "b_lights"); //!< Updating the lights UBO with the position of lights uniforms within the shader
		
		s_data->lightsUBO->uploadData("u_lightsPos", glm::value_ptr(arg_position));	//!< Uploading the position
		s_data->lightsUBO->uploadData("u_viewPos", glm::value_ptr(arg_view));		//!< Uploading the view position
		s_data->lightsUBO->uploadData("u_lightColour", glm::value_ptr(arg_colour));	//!< Uploading the colour
		s_data->lightsUBO->uploadData("u_tint", glm::value_ptr(arg_tint));			//!< Uploading the tint
	}


	void Renderer3D::beginScene()
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	}
	void Renderer3D::submit(const std::shared_ptr<VertexArray>& arg_geometry, const std::shared_ptr<Material> arg_material, const glm::mat4& arg_model)
	{
		//Could be API agnostic, put bind commands in abstracted buffers

		//Bind shader
		glUseProgram(arg_material->getShader()->getID());
		
		//Apply Uniforms
		arg_material->getShader()->uploadMat4("u_model", arg_model);

		if(arg_material->isFlagSet(Material::flag_texture)) glBindTexture(GL_TEXTURE_2D, arg_material->getTexture()->getID());
		else glBindTexture(GL_TEXTURE_2D, s_data->defaultTexture->getID());
		arg_material->getShader()->uploadInt("u_textData", 0);

		//Might not have to include this (UBOs)
		if(arg_material->isFlagSet(Material::flag_tint)) arg_material->getShader()->uploadFloat4("u_tint", arg_material->getTint());
		else arg_material->getShader()->uploadFloat4("u_tint", s_data->defaultTint);
		
		//Bind geometry (VAO and IBO)
		glBindVertexArray(arg_geometry->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arg_geometry->getIndexBuffer()->getID());
		//Submit draw call
		glDrawElements(GL_TRIANGLES, arg_geometry->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer3D::endScene()
	{
	}
}	