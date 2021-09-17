#include "shader_loader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace Poole::Rendering {

	std::string ReadFileIntoString(std::string_view filePath)
	{
		//#todo: Use the new C++17 <filesystem>
		std::string text;
		std::ifstream stream(filePath.data(), std::ios::in);
		if (stream.is_open())
		{
			std::stringstream sstr;
			sstr << stream.rdbuf();
			text = sstr.str();
			stream.close();
		}
		else
		{
			//#todo: consider puts() or external fmt::
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", filePath.data());
			getchar(); //#todo: is there a better way to keep it open?
			return ""; //#todo: use std::optional?
		}
		return text;
	}

	void CompileAndCheckShader(const GLuint& shaderID, const char* shaderCode, GLint& result, i32& infoLogLength)
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

	//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
	GLuint LoadShaderLiterals(std::string_view vertexShaderCode, std::string_view fragmentShaderCode, std::string_view vertexFilePath, std::string_view fragmentFilePath)
	{
		// Create the shaders
		const GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		const GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint result = GL_FALSE;
		i32 infoLogLength = 0;

		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertexFilePath.data());
		CompileAndCheckShader(vertexShaderID, vertexShaderCode.data(), result, infoLogLength);

		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragmentFilePath.data());
		CompileAndCheckShader(fragmentShaderID, fragmentShaderCode.data(), result, infoLogLength);

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

	GLuint LoadShaders(std::string_view vertexFilePath, std::string_view fragmentFilePath)
	{
		const std::string vertexShaderCode = ReadFileIntoString(vertexFilePath);
		const std::string fragmentShaderCode = ReadFileIntoString(fragmentFilePath);
		return LoadShaderLiterals(vertexShaderCode, fragmentShaderCode, vertexFilePath, fragmentFilePath);
	}
}

