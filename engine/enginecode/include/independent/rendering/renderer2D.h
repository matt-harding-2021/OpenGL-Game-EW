/**\ file renderer2D.h */
#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glad/glad.h>
#include <memory>
#include <string>
#include "ft2build.h"
#include "freetype/freetype.h"

#include "systems/logging.h"

#include "uniformBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "texture.h"
namespace Engine {
	class Quad {
	public:
		Quad() {
		}
		static Quad create(const glm::vec2& arg_centre, const glm::vec2& arg_halfextents) {
			Quad result;
			result.m_translate = glm::vec3(arg_centre, 0.f);
			result.m_scale = glm::vec3(arg_halfextents * 2.f, 1.f);
			return result;
		}
	private:
		glm::vec3 m_translate = glm::vec3(0.f);
		glm::vec3 m_scale = glm::vec3(1.f);
		friend class Renderer2D;
	};

	/**\ class Renderer2D
	*
	*/
	class Renderer2D {
	public:
		static void init();
		static void beginScene(bool arg_blend);
		static void uploadData(glm::mat4 arg_view, glm::mat4 arg_projection);

		static void submitQuad(const Quad& arg_quad, float arg_angle) { 
			submitQuad(arg_quad, s_data->defaultTexture, s_data->defaultTint, arg_angle); 
		}
		static void submitQuad( const Quad& arg_quad, const std::shared_ptr<Texture>& arg_texture, float arg_angle) {
			submitQuad(arg_quad, arg_texture, s_data->defaultTint, arg_angle);
		}
		static void submitQuad( const Quad& arg_quad, const glm::vec4& arg_tint, float arg_angle = s_data->defaultAngle) {
			submitQuad(arg_quad, s_data->defaultTexture, arg_tint, arg_angle);
		}
		static void submitQuad(
			const Quad& arg_quad, 
			const std::shared_ptr<Texture>& arg_texture = s_data->defaultTexture, 
			const glm::vec4& arg_tint = s_data->defaultTint, 
			float arg_angle = s_data->defaultAngle
		);

		static void submitChar(char arg_character, const glm::vec2& arg_position, float& arg_advance, const glm::vec4 arg_tint);
		static void submitText(const char* arg_text, const glm::vec2& arg_position, const glm::vec4 arg_tint);

		static void endScene();

	private:
		struct InternalData {
			std::shared_ptr<UniformBuffer> uniformBuffer;
			UniformBufferLayout UBLayout = { //!< Setting the type of data for the lights UBO
				{"u_view", ShaderDataType::Mat4},
				{"u_projection", ShaderDataType::Mat4},
			};

			glm::mat4 model;
			std::shared_ptr<Shader> shader;
			std::shared_ptr<VertexArray> vertexArray;
			unsigned char PxlColour[4] = {255, 255, 255, 255 };
			std::shared_ptr<Texture> defaultTexture;
			glm::vec4 defaultTint;
			float defaultAngle;

			FT_Library ftLibrary;
			FT_Face fontFace;
			std::shared_ptr<Texture> fontTexture;
			uint32_t fontSize = 24;

			glm::ivec2 glyphBufferDimensions;
			uint32_t glyphBufferSize;
			uint32_t glyphChannels = 4;
			std::shared_ptr<unsigned char> glyphBuffer;
		};
		static std::shared_ptr<InternalData> s_data;

		static void RtoRGBA(unsigned char* arg_Rbuffer, uint32_t arg_width, uint32_t arg_height);
	};
}