/**\ file OpenGLShader.cpp */
#include "engine_pch.h"
#include "platform/OpenGL/OpenGLShader.h"
#include "systems/logging.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
namespace Engine 
{
	/**\ Opens and reads the file*/
	OpenGLShader::OpenGLShader(const char* arg_VerFilepath, const char* arg_FragFilepath)
	{
		src[Region::R_VERTEX] = readFile(arg_VerFilepath, src)[Region::R_VERTEX];
		src[Region::R_FRAGMENT] = readFile(arg_FragFilepath, src)[Region::R_FRAGMENT];
		compileAndLink(src[Region::R_VERTEX].c_str(), src[Region::R_FRAGMENT].c_str()); //!< If both files have been read successfully, we can compile the Src strings (now containing the shader programs)

	}
	OpenGLShader::OpenGLShader(const char* arg_Filepath)
	{
		std::array<std::string, Region::R_COMPUTE + 1> src; //!< Strings to hold the body of text from each file
		src = readFile(arg_Filepath, src); 
		compileAndLink(src[Region::R_VERTEX].c_str(), src[Region::R_FRAGMENT].c_str()); //!< If both files have been read successfully, we can compile the Src strings (now containing the shader programs)
	}

	/** Cleaning up memory on application exit
	*	Deletes the OpenGL shader programs
	*/
	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_OpenGL_ID);
	}
	
	std::array<std::string, Region::R_COMPUTE + 1> OpenGLShader::readFile(const char* arg_Filepath, std::array<std::string, Region::R_COMPUTE + 1> arg_FileSrc) //!< Reading the filepath
	{
		uint32_t currentRegion = Region::R_NONE;
		std::string line; //!< String to hold a line of text from the file

		std::fstream handle(arg_Filepath, std::ios::in); //!< Creates the file reader
		if (handle.is_open()) { //!< If the file path is ok
			while (std::getline(handle, line)) { //!< Loops through the file line by line, and accumulates it
				if (line.find("#region Vertex") != std::string::npos) { currentRegion = Region::R_VERTEX; continue; } //!< Continue causes the loop to immediately jump to the next iteration of the loop
				if (line.find("#region Fragment") != std::string::npos) { currentRegion = Region::R_FRAGMENT; continue; }
				if (line.find("#region Geometry") != std::string::npos) { currentRegion = Region::R_GEOMETRY; continue; }
				if (line.find("#region TessellationControl") != std::string::npos) { currentRegion = Region::R_TESSELLATIONCONTROL; continue; }
				if (line.find("#region TessellationEvaluation") != std::string::npos) { currentRegion = Region::R_TESSELLATIONEVALUATION; continue; }
				if (line.find("#region Compute") != std::string::npos) { currentRegion = Region::R_COMPUTE; continue; }
				if (currentRegion != Region::R_NONE) arg_FileSrc[currentRegion] += (line + "\n");
			} 
			handle.close();
		}
		else {
			LOG_ERROR("Could not open shader filepath: {0}", arg_Filepath); //!< Logs the error to console if the filepath can't be used
		}
		return arg_FileSrc;
	}
	void OpenGLShader::compileAndLink(const char* arg_VerShaderSrc, const char* arg_FragShaderSrc)
	{

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = arg_VerShaderSrc;
		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(vertexShader);
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		source = arg_FragShaderSrc;
		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader compile error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			return;
		}

		m_OpenGL_ID = glCreateProgram();
		glAttachShader(m_OpenGL_ID, vertexShader);
		glAttachShader(m_OpenGL_ID, fragmentShader);
		glLinkProgram(m_OpenGL_ID);

		GLint isLinked = 0;
		glGetProgramiv(m_OpenGL_ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_OpenGL_ID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_OpenGL_ID, maxLength, &maxLength, &infoLog[0]);
			LOG_ERROR("Shader linking error: {0}", std::string(infoLog.begin(), infoLog.end()));

			glDeleteProgram(m_OpenGL_ID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			return;
		}

		glDetachShader(m_OpenGL_ID, vertexShader);
		glDetachShader(m_OpenGL_ID, fragmentShader);
	}

	void OpenGLShader::uploadInt(const char* arg_Name, int arg_Value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, arg_Name);
		glUniform1i(uniformLocation, arg_Value);
	}
	void OpenGLShader::uploadFloat(const char* arg_Name, float arg_Value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, arg_Name);
		glUniform1f(uniformLocation, arg_Value);
	}
	void OpenGLShader::uploadFloat2(const char* arg_Name, const glm::vec2& arg_Value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, arg_Name);
		glUniform2f(uniformLocation, arg_Value.x, arg_Value.y);
	}
	void OpenGLShader::uploadFloat3(const char* arg_Name, const glm::vec3& arg_Value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, arg_Name);
		glUniform3f(uniformLocation, arg_Value.x, arg_Value.y, arg_Value.z);
	}
	void OpenGLShader::uploadFloat4(const char* arg_Name, const glm::vec4& arg_Value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, arg_Name);
		glUniform4f(uniformLocation, arg_Value.x, arg_Value.y, arg_Value.z, arg_Value.w);
	}
	void OpenGLShader::uploadMat4(const char* arg_Name, const glm::mat4& arg_Value)
	{
		uint32_t uniformLocation = glGetUniformLocation(m_OpenGL_ID, arg_Name);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(arg_Value)); 
	}
}