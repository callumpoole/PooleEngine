#pragma once

#include "poole/core.h"
#include "rendering/graphics_api/texture.h"

namespace Poole::Rendering
{
	class GLShader;


	//A lot of this code was sources by VictorGordan: https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%206%20-%20Textures
	class OpenGL_Texture : public Texture
	{
	public:
		/// <param name="image">Path</param>
		/// <param name="texType">GL_TEXTURE_2D</param>
		/// <param name="slot">GL_TEXTURE0</param>
		/// <param name="format">GL_RGBA</param>
		/// <param name="pixelType">GL_UNSIGNED_BYTE</param>
		OpenGL_Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

		/// <param name="shaderID">shaderProgramID</param>
		/// <param name="uniform">"tex0"</param>
		/// <param name="unit">0</param>
		virtual void SetTextureUnit(GLShader& shader, const char* uniform, GLuint unit) override;

		virtual ~OpenGL_Texture();

		virtual void Bind();
		virtual void Unbind();

	private:
		GLuint ID;
		GLenum type;
	};
}
