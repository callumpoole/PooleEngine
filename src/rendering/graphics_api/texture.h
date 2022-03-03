
#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class GLShader;

	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		/// <param name="image">Path</param>
		/// <param name="texType">GL_TEXTURE_2D</param>
		/// <param name="slot">GL_TEXTURE0</param>
		/// <param name="format">GL_RGBA</param>
		/// <param name="pixelType">GL_UNSIGNED_BYTE</param>
		static Texture* Create(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

		/// <param name="shaderID">shaderProgramID</param>
		/// <param name="uniform">"tex0"</param>
		/// <param name="unit">0</param>
		virtual void SetTextureUnit(GLShader& /*shader*/, const char* /*uniform*/, GLuint /*unit*/) {}
	};
}
