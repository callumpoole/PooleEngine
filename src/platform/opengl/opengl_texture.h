#pragma once

#include "poole/core.h"
#include "rendering/graphics_api/texture.h"

namespace Poole::Rendering
{
	class GLShader;

	//A lot of this code was sources by VictorGordan: https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%206%20-%20Textures
	class OpenGL_Texture : public Texture
	{
		//===BOTH===
	public:
		virtual ~OpenGL_Texture();
		virtual uvec2 GetSize() const override { return m_Size; }
	private:
		GLuint m_ID;
		uvec2 m_Size;
		GLenum m_TargetType;
		GLenum m_ShaderFormat;
		GLenum m_DataFormat;
		GLenum m_PixelType;

		void SetupTextureParams();

		//===OLD===

	public:
		/// <param name="imagePath">Path</param>
		/// <param name="slot">GL_TEXTURE0</param>
		OpenGL_Texture(const char* imagePath, GLenum slot);
		OpenGL_Texture(const Image& image, GLenum slot);

		/// <param name="shaderID">shaderProgramID</param>
		/// <param name="uniform">"tex0"</param>
		/// <param name="unit">0</param>
		virtual void SetTextureUnit(GLShader& shader, const char* uniform, GLuint unit) override;

		virtual void Bind() override;
		virtual void Unbind() override;

		//===NEW===
	public:
		OpenGL_Texture(uvec2 size);
		virtual void SetData(const void* data, u32 size) override;
		virtual void Bind(u32 slot) override;
		virtual void Unbind(u32 slot) override;
	};
}
