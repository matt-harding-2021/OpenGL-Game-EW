/**\ file renderer3D.h */
#pragma once
#include <unordered_map>

#include "glm/glm.hpp"

#include "vertexArray.h"
#include "texture.h"
#include "shader.h"
#include "shaderDataType.h"
#include "renderAPI.h"

#include "uniformBuffer.h"
#include "subTexture.h"

namespace Engine {
	using SceneWideUniforms = std::unordered_map<const char*, std::pair<ShaderDataType, void*>>;
	/**\ Class Material 
	*	 Holds a shader and uniform data associated
	*/
	class Material {
	public:
		/**\ Constructor that takes only a shader (necessary for a material) */
		Material(const std::shared_ptr<Shader>& arg_shader) : m_shader(arg_shader), m_flags(0), m_texture(nullptr), m_tint(glm::vec4(0.f)) {

		}
		/**\ Constructor that takes a texture */
		Material(const std::shared_ptr<Shader>& arg_shader, const std::shared_ptr<Texture> arg_texture) : m_shader(arg_shader), m_texture(arg_texture), m_tint(glm::vec4(0.f)) {
			setFlag(flag_texture);
		}
		/**\ Constructor that takes a tint */
		Material(const std::shared_ptr<Shader>& arg_shader, const glm::vec4 arg_tint) : m_shader(arg_shader), m_texture(nullptr), m_tint(arg_tint) {
			setFlag(flag_tint);
		}
		/**\ Constructor that takes each of the components */
		Material(const std::shared_ptr<Shader>& arg_shader, const std::shared_ptr<Texture> arg_texture, const glm::vec4 arg_tint) : m_shader(arg_shader), m_texture(arg_texture), m_tint(arg_tint) {
			setFlag(flag_texture | flag_tint);
		}

		inline std::shared_ptr<Shader> getShader() const { return m_shader; } //!< Returns the shader 
		inline std::shared_ptr<Texture> getTexture() const { return m_texture; } //!< Returns the texture
		inline glm::vec4 getTint() const { return m_tint; } //!< Returns the tint

		/**\ Bitwise AND, returns false if none of the values match
		*
		*		0000 0101 
		*	AND 0000 0011
		*	  = 0000 0001
		*/
		bool isFlagSet(uint32_t arg_flag) const { return m_flags & arg_flag; } 

		void setShader(const std::shared_ptr<Shader>& arg_shader) { m_shader = arg_shader; }
		void setTexture(const std::shared_ptr<Texture>& arg_texture) { m_texture = arg_texture; }
		void setTint(const glm::vec4& arg_tint) { m_tint = arg_tint; }
		
		constexpr static uint32_t flag_texture = 1 << 0; //!< 00000001. constexpr lets the compiler calculate it at compile time. 
		//Specular tex
		//Reflective tex
		//Omissive tex
		//Normal tex
		//params struct with shrdptr for each texture
		//set flags for which ones needed at runtime
		//flags would decide whether or not the texture is initialised or set to nullptr
		constexpr static uint32_t flag_tint = 1 << 1;	 //!< 00000010
	private:
		/**	 Shader settings - Bitwise Operators
		*	 C++ lets integer members be stored into memory spaces smaller than the compiler normally allows, called bitfields
		*	 Bitfields save space as they remove the padding you would normally get with declaring a member variable
		*
		*	 In this case, the bit fields are used to add bits depending on whether a shader setting is true, same principle as in event.h
		*	 e.g. 
		*	 00000001 for one setting being true can go to:
		*	 00000011 for two settings being true.
		*
		*	 Also gives the option of default colours and textures
		*/
		uint32_t m_flags = 0;

		void setFlag(uint32_t arg_flag) { m_flags |= arg_flag; } //!< If the flag passed is different, add it using bitwise addition 

		std::shared_ptr<Shader> m_shader; //!< Shader for the material
		std::shared_ptr<Texture> m_texture; //!< Texture associated
		glm::vec4 m_tint;
	};
	/**\ Class Renderer3d 
	*/
	class Renderer3D
	{
	public:
		static void init(); //!< Initializes the renderer
		static void beginScene(const SceneWideUniforms& arg_uniforms);
		static void submit(const std::shared_ptr<VertexArray>& arg_geometry, const std::shared_ptr<Material> arg_material, const glm::mat4& arg_model);
		static void endScene();
	private:
		struct InternalData
		{
			SceneWideUniforms uniforms;
			//UniformBuffer uniformBuffer;
			std::shared_ptr<Texture> texture;
			//SubTexture texture;
			glm::vec4 tint;
		};
		static std::shared_ptr<InternalData> s_data; //!< One set of data per application. It is private so only this class can edit the data.
	}; 
}