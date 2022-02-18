#include "poole/rendering/shader_loader.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "glm/gtc/type_ptr.hpp"

namespace Poole::Rendering {

	GLShader::GLShader(std::string_view combinedPath)
	{
		bool Success = true;
		std::optional<ShaderSource> Source = LoadFromFile(combinedPath);
		if (Source)
		{
			m_programID = LoadShaderLiterals(Source->vertexShader.c_str(), Source->fragmentShader.c_str(), Success);

			if (Success)
			{
				LOG("Loaded {}", combinedPath.data());
			}
		}
	}
	GLShader::GLShader(std::string_view vertexShaderCode, std::string_view fragmentShaderCode)
	{
		bool Success = true;
		m_programID = LoadShaderLiterals(vertexShaderCode.data(), fragmentShaderCode.data(), Success);
		if (Success)
		{
			LOG("Loaded Vertex: {} & Fragment: {}", vertexShaderCode.data(), fragmentShaderCode.data());
		}
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
		//LOG_LINE("Loading shader: {}", combinedPath.data());

		namespace fs = std::filesystem;
		fs::path path = fs::absolute(fs::path(combinedPath));
		if (fs::exists(path))
		{
			std::ifstream stream(path, std::ios::in);
			if (stream.is_open())
			{
				constexpr u8 NUM_TYPES = 2;
				std::stringstream ss[NUM_TYPES];
				
				enum class EShaderType : u8
				{
					Vertex = 0, //Has to be 0 since it's an array index
					Fragment = 1,
					Last = Fragment,
					None = 255,
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

					static_assert(NUM_TYPES == 2);
					if ((u8)type == 0)
					{
						ss[1] << '\n'; //This ensures line numbers are correct in error messages
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

	GLuint GLShader::LoadShaderLiterals(const char* vertexShaderCode, const char* fragmentShaderCode, bool& outSuccess)
	{
		//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

		outSuccess = true;

		// Create the shaders
		const GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint result = GL_FALSE;
		i32 infoLogLength = 0;

		auto ShowError = [&infoLogLength](const GLuint& ID, std::string_view shaderTypeName)
		{
			std::vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(ID, infoLogLength, NULL, &shaderErrorMessage[0]);
			LOG_ERROR("Failed to {} - {}", shaderTypeName.data(), &shaderErrorMessage[0]);
		};

		// Compile Vertex Shader
		const bool CompiledVertex = CompileShader(vertexShaderID, vertexShaderCode, result, infoLogLength);
		if (!CompiledVertex)
		{
			ShowError(vertexShaderID, "Compile Vertex Shader");
			outSuccess = false;
		}

		// Compile Fragment Shader
		const bool CompiledFragment = CompileShader(fragmentShaderID, fragmentShaderCode, result, infoLogLength);
		if (!CompiledFragment)
		{
			ShowError(fragmentShaderID, "Compile Fragment Shader");
			outSuccess = false;
		}

		// Link the program
		const GLuint programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);

		// Check the program
		glGetProgramiv(programID, GL_LINK_STATUS, &result);
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			ShowError(programID, "Link Program ID");
			outSuccess = false;
		}

		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return programID;
	}

	bool GLShader::CompileShader(const GLuint& shaderID, const char* shaderCode, GLint& result, i32& infoLogLength)
	{
		glShaderSource(shaderID, 1, &shaderCode, NULL);
		glCompileShader(shaderID);

		// Check Vertex Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		return infoLogLength == 0;
	}
}

