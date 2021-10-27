#pragma once
#include "poole/core.h"

namespace Poole::Rendering {

	class Shader
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetUniform(const char* uniformName, const f32 f) = 0;
		virtual void SetUniform(const char* uniformName, const fvec2 v) = 0;
		virtual void SetUniform(const char* uniformName, const fvec3 v) = 0;
		virtual void SetUniform(const char* uniformName, const fvec4 v) = 0;
		virtual void SetUniform(const char* uniformName, const fmat2 v) = 0;
		virtual void SetUniform(const char* uniformName, const fmat3 v) = 0;
		virtual void SetUniform(const char* uniformName, const fmat4 v) = 0;
	};

	class GLShader : public Shader
	{
	public:
		 GLShader() = default;
		 GLShader(std::string_view combinedPath);
		 GLShader(std::string_view vertexShaderCode, std::string_view fragmentShaderCode);
		 ~GLShader();
		 GLuint GetProgramID() const { return m_programID; }

		 virtual void Bind() override;
		 virtual void Unbind() override;
		 virtual void SetUniform(const char* uniformName, const f32 f);
		 virtual void SetUniform(const char* uniformName, const fvec2 v);
		 virtual void SetUniform(const char* uniformName, const fvec3 v);
		 virtual void SetUniform(const char* uniformName, const fvec4 v);
		 virtual void SetUniform(const char* uniformName, const fmat2 v);
		 virtual void SetUniform(const char* uniformName, const fmat3 v);
		 virtual void SetUniform(const char* uniformName, const fmat4 v);
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
