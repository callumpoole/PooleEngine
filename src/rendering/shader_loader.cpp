#include "shader_loader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

namespace Poole { namespace Rendering {

	std::string ReadFileIntoString(std::string_view file_path)
	{
		std::string text;
		std::ifstream stream(file_path.data(), std::ios::in);
		if (stream.is_open()) {
			std::stringstream sstr;
			sstr << stream.rdbuf();
			text = sstr.str();
			stream.close();
		}
		else {
			printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", file_path.data());
			getchar();
			return 0;
		}
		return text;
	}

	void CompileAndCheckShader(GLuint& shaderID, const char* shaderCode, GLint& result, int& infoLogLength)
	{
		glShaderSource(shaderID, 1, &shaderCode, NULL);
		glCompileShader(shaderID);

		// Check Vertex Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> shaderErrorMessage(infoLogLength + 1);
			glGetShaderInfoLog(shaderID, infoLogLength, NULL, &shaderErrorMessage[0]);
			printf("%s\n", &shaderErrorMessage[0]);
		
		}
	}

	//Source: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
	GLuint LoadShaderLiterals(std::string_view vertexShaderCode, std::string_view fragmentShaderCode, std::string_view vertex_file_path, std::string_view fragment_file_path)
	{
		// Create the shaders
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint result = GL_FALSE;
		int infoLogLength;

		// Compile Vertex Shader
		printf("Compiling shader : %s\n", vertex_file_path.data());
		CompileAndCheckShader(vertexShaderID, vertexShaderCode.data(), result, infoLogLength);

		// Compile Fragment Shader
		printf("Compiling shader : %s\n", fragment_file_path.data());
		CompileAndCheckShader(fragmentShaderID, fragmentShaderCode.data(), result, infoLogLength);

		// Link the program
		printf("Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vertexShaderID);
		glAttachShader(ProgramID, fragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(infoLogLength + 1);
			glGetProgramInfoLog(ProgramID, infoLogLength, NULL, &ProgramErrorMessage[0]);
			printf("%s\n", &ProgramErrorMessage[0]);
		}

		glDetachShader(ProgramID, vertexShaderID);
		glDetachShader(ProgramID, fragmentShaderID);

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		return ProgramID;
	}

	GLuint LoadShaders(std::string_view vertex_file_path, std::string_view fragment_file_path)
	{
		std::string vertexShaderCode = ReadFileIntoString(vertex_file_path);
		std::string fragmentShaderCode = ReadFileIntoString(fragment_file_path);
		return LoadShaderLiterals(vertexShaderCode, fragmentShaderCode, vertex_file_path, fragment_file_path);
	}
}}

