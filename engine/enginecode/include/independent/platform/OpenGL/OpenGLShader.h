/**\ file OpenGLShader.h */
#pragma once

#include <glm/glm.hpp>

#include "rendering/shader.h"
namespace Engine
{
	/** Class OpenGLShader 
	*	Takes in text and compiles it to a shader program
	*/
	enum Region { R_NONE = -1, R_VERTEX = 0, R_FRAGMENT, R_GEOMETRY, R_TESSELLATIONCONTROL, R_TESSELLATIONEVALUATION, R_COMPUTE };
	class OpenGLShader //: public Shader //API AGNOSTIC
	{
	public:
		OpenGLShader(const char* arg_VerFilepath, const char* arg_FragFilepath); //!< Constructor takes a vertex filepath, and a fragment filepath
		OpenGLShader(const char* arg_Filepath); //!< Gives the option of a single file containing both shaders
		~OpenGLShader();

		/* API AGNOSTIC VERSION
		virtual uint32_t getID() const override { return m_OpenGL_ID; }

		virtual void uploadInt(const char* arg_Name, int arg_Value) override;
		virtual void uploadFloat(const char* arg_Name, float arg_Value) override;
		virtual void uploadFloat2(const char* arg_Name, const glm::vec2& arg_Value) override;
		virtual void uploadFloat3(const char* arg_Name, const glm::vec3& arg_Value) override;
		virtual void uploadFloat4(const char* arg_Name, const glm::vec4& arg_Value) override;
		virtual void uploadMat4(const char* arg_Name, const glm::mat4& arg_Value) override;
		*/

		uint32_t getID() const { return m_OpenGL_ID; }

		void uploadInt(const char* arg_Name, int arg_Value);
		void uploadFloat(const char* arg_Name, float arg_Value);
		void uploadFloat2(const char* arg_Name, const glm::vec2& arg_Value);
		void uploadFloat3(const char* arg_Name, const glm::vec3& arg_Value);
		void uploadFloat4(const char* arg_Name, const glm::vec4& arg_Value);
		void uploadMat4(const char* arg_Name, const glm::mat4& arg_Value);
	private:
		uint32_t m_OpenGL_ID;

		std::array<std::string, Region::R_COMPUTE + 1> src; //!< Strings to hold the body of text from each file

		std::array<std::string, Region::R_COMPUTE + 1> readFile(const char* arg_Filepath, std::array<std::string, Region::R_COMPUTE + 1> arg_FileSrc);

		void compileAndLink(const char* arg_VerShaderSrc, const char* arg_FragShaderSrc); //!< Private function 
	};
}