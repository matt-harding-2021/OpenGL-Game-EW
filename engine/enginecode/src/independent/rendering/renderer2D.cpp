/**\ file renderer2D.cpp */

#include "engine_pch.h"
#include "rendering/renderer2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {
	std::shared_ptr<Renderer2D::InternalData> Renderer2D::s_data = nullptr;
	void Renderer2D::init()
	{
		s_data.reset(new InternalData);
		s_data->shader.reset(Shader::create("./assets/shaders/Shader2D.glsl"));
		s_data->defaultTexture.reset(Texture::create(1, 1, 4, s_data->PxlColour));
		s_data->defaultTint = { 1.f, 1.f, 1.f, 1.f };
		s_data->defaultAngle = 0.f;
		s_data->model = glm::mat4(1.0f);

		float vertices[4 * 4] = {
			-0.5f,	-0.5f,	0.f,	0.f,
			-0.5f,	0.5f,	0.f,	1.f,
			0.5f,	0.5f,	1.f,	1.f,
			0.5f,	-0.5f,	1.f,	0.f
		};
		uint32_t indices[4] = { 0, 1, 2, 3 };

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices), VertexBufferLayout({ ShaderDataType::Float2, ShaderDataType::Float2 })));

		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::create(indices, 4));

		s_data->vertexArray.reset(VertexArray::create());
		s_data->vertexArray->addVertexBuffer(vertexBuffer);
		s_data->vertexArray->setIndexBuffer(indexBuffer);


		s_data->glyphBufferDimensions = { 256, 256 };
		s_data->glyphBufferSize =
			s_data->glyphBufferDimensions.x * s_data->glyphBufferDimensions.y //<! Number of pixels
			*
			s_data->glyphChannels //<! Number of channels
			*
			sizeof(unsigned char); //!< Size of the data being stored
		s_data->glyphBuffer.reset(static_cast<unsigned char*>(malloc(s_data->glyphBufferSize))); //!< Manually allocate memory

		const char* fontFilepath = "./assets/fonts/arial.ttf";
		if (FT_Init_FreeType(&s_data->ftLibrary)) LOG_ERROR("Error: Init Freetype in Renderer2D");
		if (FT_New_Face(s_data->ftLibrary, fontFilepath, 0, &s_data->fontFace)) LOG_ERROR("Error: Could not load font: {0}", fontFilepath);
		if (FT_Set_Pixel_Sizes(s_data->fontFace, 0, s_data->fontSize)) LOG_ERROR("Error: Could not set font size: {0}", s_data->fontSize);
		s_data->fontTexture.reset(Texture::create(s_data->glyphBufferDimensions.x, s_data->glyphBufferDimensions.y, s_data->glyphChannels, nullptr));

		//giving the buffer some initial values
		memset(s_data->glyphBuffer.get(), 60, s_data->glyphBufferSize); //.get gets the c ptr.  fills the glyphBuffer with the value passed
		s_data->fontTexture->edit({ 0, 0 }, s_data->glyphBufferDimensions, s_data->glyphChannels, s_data->glyphBuffer.get());
	}
	void Renderer2D::uploadData(glm::mat4 arg_view, glm::mat4 arg_projection)
	{
		s_data->uniformBuffer.reset(UniformBuffer::create(s_data->UBLayout));
		s_data->uniformBuffer->attachShaderBlock(s_data->shader, "b_uniforms"); //!< Updating the camera UBO with the position of camera uniforms within the shader

		s_data->uniformBuffer->uploadData("u_view", glm::value_ptr(arg_view));
		s_data->uniformBuffer->uploadData("u_projection", glm::value_ptr(arg_projection));
	}
	void Renderer2D::beginScene(bool arg_blend)
	{
		glDisable(GL_DEPTH_TEST);
		if (arg_blend) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else glDisable(GL_BLEND);
	}
	void Renderer2D::submitQuad(
		const Quad& arg_quad, 
		const std::shared_ptr<Texture>& arg_texture /*= s_data->defaultTexture*/, 
		const glm::vec4& arg_tint /*= s_data->defaultTint*/, 
		float arg_angle /*= s_data->defaultAngle*/
	){
		glUseProgram(s_data->shader->getID());

		glBindTexture(GL_TEXTURE_2D, arg_texture->getID());
		//glm::rotate(mat, angle, axis(Z) )
		s_data->model = glm::translate(glm::mat4(1.f), arg_quad.m_translate);
		s_data->model = glm::rotate(s_data->model, glm::radians(arg_angle), { 0.f, 0.f, 1.f });
		s_data->model = glm::scale(s_data->model, arg_quad.m_scale); // needs friend class renderer2d for m_translate and m_scale
		s_data->shader->uploadMat4("u_model", s_data->model);
		s_data->shader->uploadInt("u_texData", 0);
		s_data->shader->uploadFloat4("u_tint", arg_tint);


		
		glBindVertexArray(s_data->vertexArray->getID());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data->vertexArray->getIndexBuffer()->getID());
		

		glDrawElements(GL_QUADS, s_data->vertexArray->getDrawCount(), GL_UNSIGNED_INT, nullptr);
	}
	void Renderer2D::submitChar(char arg_character, const glm::vec2& arg_position, float& arg_advance, const glm::vec4 arg_tint)
	{
		if (FT_Load_Char(s_data->fontFace, arg_character, FT_LOAD_RENDER)) LOG_ERROR("Error: Could not load char in Renderer2D");
		else {
			//get glyph data 
			glm::vec2 glyphSize(s_data->fontFace->glyph->bitmap.width, s_data->fontFace->glyph->bitmap.rows);// number of rows in the bitmap = height
			glm::vec2 glyphBearing(s_data->fontFace->glyph->bitmap_left, -s_data->fontFace->glyph->bitmap_top);// -top gives the offset of the bearing (i.e the topleft of the char)
			//calculate the advance
			arg_advance = static_cast<float>(s_data->fontFace->glyph->advance.x >> 6);// Bitshifting by 6 divides the result by 64. Advance is measured in 1/64th pixels

			//calculate the quad for the glyph
			glm::vec2 glyphHalfExtents = s_data->fontTexture->getSize() * 0.5f;
			glm::vec2 glyphCentre = (arg_position + glyphBearing) + glyphHalfExtents; // finds the position and moves across by half the width and height

			RtoRGBA(s_data->fontFace->glyph->bitmap.buffer, glyphSize.x, glyphSize.y);
			s_data->fontTexture->edit({ 0,0 }, s_data->glyphBufferDimensions, s_data->glyphChannels, s_data->glyphBuffer.get());

			submitQuad(Quad::create(glyphCentre, glyphHalfExtents), s_data->fontTexture, arg_tint);
		}
	}
	void Renderer2D::submitText(const char* arg_text, const glm::vec2& arg_position, const glm::vec4 arg_tint)
	{
		uint32_t length = strlen(arg_text);
		glm::vec2 position = arg_position;
		float advance = 0.f;
		for (int i = 0; i < length; i++) {
			submitChar(arg_text[i], position, advance, arg_tint);
			position.x += advance;
		}
	}
	void Renderer2D::RtoRGBA(unsigned char* arg_Rbuffer, uint32_t arg_width, uint32_t arg_height)
	{
		memset(s_data->glyphBuffer.get(), 0, s_data->glyphBufferSize);
		unsigned char* p = s_data->glyphBuffer.get();
		for (int32_t i = 0; i < arg_height; i++) {
			for (int32_t j = 0; j < arg_width; j++) {
				*p = 255; //white R
				p++;
				*p = 255; //white G
				p++;
				*p = 255; //white B
				p++;
				*p = *arg_Rbuffer; // Set alpha channel
				p++; //R
				arg_Rbuffer++; //next monchrome pixel
			}
			p += (s_data->glyphBufferDimensions.x - arg_width) * s_data->glyphChannels;
		}
	} 
	void Renderer2D::endScene()
	{
	}
}