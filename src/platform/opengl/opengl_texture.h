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

		//===OLD===

	public:
		/// <param name="imagePath">Path</param>
		/// <param name="texType">GL_TEXTURE_2D</param>
		/// <param name="slot">GL_TEXTURE0</param>
		/// <param name="format">GL_RGBA</param>
		/// <param name="pixelType">GL_UNSIGNED_BYTE</param>
		OpenGL_Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
		OpenGL_Texture(const Image& image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

		/// <param name="shaderID">shaderProgramID</param>
		/// <param name="uniform">"tex0"</param>
		/// <param name="unit">0</param>
		virtual void SetTextureUnit(GLShader& shader, const char* uniform, GLuint unit) override;


		//Deprecated
		virtual void Bind() override;
		//Deprecated
		virtual void Unbind() override;

	private:
		GLenum m_Type;


		//===NEW===
	public:
		OpenGL_Texture(u32 width, u32 height);
		virtual void SetData(const void* data, u32 size) override;
		virtual void Bind(u32 slot);

		GLenum m_InternalFormat;
		GLenum m_DataFormat;
	};
}
