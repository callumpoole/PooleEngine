#pragma once
#include "core.h"
#include <string_view>
#include <string>
#include <optional>

namespace Poole::Rendering {

	class GLShader
	{
	public:
		GLShader() = default;
		GLShader(std::string_view combinedPath);
		GLShader(std::string_view vertexShaderCode, std::string_view fragmentShaderCode);
		~GLShader();
		GLuint GetProgramID() const { return m_programID; }
	private:
		struct ShaderSource
		{
			std::string vertexShader;
			std::string fragmentShader;
		};

		std::optional<ShaderSource> LoadFromFile(std::string_view combinedPath);

		GLuint LoadShaderLiterals(const char* vertexShaderCode, const char* fragmentShaderCode);
		void CompileAndCheckShader(const GLuint& shaderID, const char* shaderCode, GLint& result, i32& infoLogLength);

		GLuint m_programID = 0;
	};
}
