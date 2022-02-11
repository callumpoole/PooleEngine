#include "poole/rendering/shader_loader.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "glm/gtc/type_ptr.hpp"

namespace Poole::Rendering {

	GLShader::GLShader(std::string_view combinedPath)
	{
		std::optional<ShaderSource> Source = LoadFromFile(combinedPath);
		if (Source)
		{
			m_programID = LoadShaderLiterals(Source->vertexShader.c_str(), Source->fragmentShader.c_str());
		}
	}
	GLShader::GLShader(std::string_view vertexShaderCode, std::string_view fragmentShaderCode)
	{
		m_programID = LoadShaderLiterals(vertexShaderCode.data(), fragmentShaderCode.data());
	}
	GLShader::~GLShader()
	{
	}

	void GLShader::Bind() { glUseProgram(m_programID); }
	void GLShader::Unbind() { glUseProgram(0); }
	void GLShader::SetUniform(const char* uniformName, const f32 f) { glUniform1f(glGetUniformLocation(m_programID, uniformName), f); }
	void GLShader::SetUniform(const char* uniformName, const fvec2 v) { glUniform2f(glGetUniformLocation(m_programID, uniformName), v.x, v.y); }
	void GLShader::SetUniform(const char* uniformName, const fvec3 v) { glUniform3f(glGetUniformLocation(m_programID, uniformName), v.x, v.y, v.z); }
	void GLShader::SetUniform(const char* uniformName, const fvec4 v) { glUniform4f(glGetUniformLocation(m_programID, uniformName), v.x, v.y, v.z, v.w); }
	void GLShader::SetUniform(const char* uniformName, const fmat2 v) { glUniformMatrix2fv(glGetUniformLocation(m_programID, uniformName), 1, GL_FALSE, glm::value_ptr(v)); }
	void GLShader::SetUniform(const char* uniformName, const fmat3 v) { glUniformMatrix3fv(glGetUniformLocation(m_programID, uniformName), 1, GL_FALSE, glm::value_ptr(v)); }
	void GLShader::SetUniform(const char* uniformName, const fmat4 v) { glUniformMatrix4fv(glGetUniformLocation(m_programID, uniformName), 1, GL_FALSE, glm::value_ptr(v)); }

	std::optional<GLShader::ShaderSource> GLShader::LoadFromFile(std::string_view combinedPath)
	{
		LOG_LINE("Loading shader: {}", combinedPath.data());

		namespace fs = std::filesystem;
		fs::path path = fs::absolute(fs::path(combinedPath));
		if (fs::exists(path))
		{
			std::ifstream stream(path, std::ios::in);
			if (stream.is_open())
			{
				std::stringstream ss[2];

				enum class EShaderType : u8
				{
					None = -1,
					Vertex = 0,
					Fragment = 1
				};

				std::string line;
				EShaderType type = EShaderType::None;
				while (getline(stream, line))
				{
					if (line.find("#type") != std::string::npos)
					{
						if (line.find("vertex") != std::string::npos)
							type = EShaderType::Vertex;
						else if (line.find("fragment") != std::string::npos)
							type = EShaderType::Fragment;
					}
					else if (type != EShaderType::None)
					{
						ss[(u8)type] << line << '\n';
					}
				}
				stream.close();

				return ShaderSource { 
					ss[(u8)EShaderType::Vertex].str(), 
					ss[(u8)EShaderType::Fragment].str() 
				};
			}
		}

		LOG_FATAL("Impossible to open {}. Are you in the right directory ? Don't forget to read the FAQ !", path.string());
		
		return std::nullopt;
	}

	GLuint GLShader::LoadShaderLiterals(const char* vertexShaderCode, const char* fragmentShaderCode)
	{
		//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

		// Create the shaders
		const GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint result = GL_FALSE;
		i32 infoLogLength = 0;

		// Compile Vertex Shader
		LOG_LINE("Compiling shader: Vertex");
		CompileAndCheckShader(vertexShaderID, vertexShaderCode, result, infoLogLength);

		// Compile Fragment Shader
		LOG_LINE("Compiling shader: Fragment");
		CompileAndCheckShader(fragmentShaderID, fragmentShaderCode, result, infoLogLength);

		// Link the program
		LOG_LINE("Linking program");
		const GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		// Check the program
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> ProgramErrorMessage(infoLogLength + 1); //#todo: Investigate this, +1 for \0 ???
			glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
			LOG_ERROR("{}", &ProgramErrorMessage[0]);
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return programID;
	}

	void GLShader::CompileAndCheckShader(const GLuint& shaderID, const char* shaderCode, GLint& result, i32& infoLogLength)
	{
		glShaderSource(shaderID, 1, &shaderCode, NULL);
		glCompileShader(shaderID);

		// Check Vertex Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			std::vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
			LOG_ERROR("{}", &shaderErrorMessage[0]);
		}
	}
}

