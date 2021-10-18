#include "poole/rendering/shader_loader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>

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
	
	std::optional<GLShader::ShaderSource> GLShader::LoadFromFile(std::string_view combinedPath)
	{
		printf("Loading shader : %s\n", combinedPath.data());

		namespace fs = std::filesystem;
		fs::path path = fs::absolute(fs::path(combinedPath));
		if (fs::exists(path))
		{
			std::ifstream stream(path, std::ios::in);
			if (stream.is_open())
			{
				std::stringstream ss[2];

				enum class EShaderType
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
						ss[(int)type] << line << '\n';
					}
				}
				stream.close();
				return ShaderSource{ ss[0].str(), ss[1].str() };
			}
		}

		//#todo: consider puts() or external fmt::
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", path.c_str());
		getchar(); //#todo: is there a better way to keep it open?

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
		printf("Compiling shader : Vertex\n");
		CompileAndCheckShader(vertexShaderID, vertexShaderCode, result, infoLogLength);

		// Compile Fragment Shader
		printf("Compiling shader : Fragment\n");
		CompileAndCheckShader(fragmentShaderID, fragmentShaderCode, result, infoLogLength);

		// Link the program
		printf("Linking program\n");
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
			printf("%s\n", &ProgramErrorMessage[0]);
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
			printf("%s\n", &shaderErrorMessage[0]);
		}
	}
}

