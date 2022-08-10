
#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class Image;
	class GLShader;

	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void Bind(u32 slot) = 0;

		//Deprecated
		virtual void Bind() = 0;
		//Deprecated
		virtual void Unbind() = 0;

		/// <param name="image">Path</param>
		/// <param name="texType">GL_TEXTURE_2D</param>
		/// <param name="slot">GL_TEXTURE0</param>
		/// <param name="format">GL_RGBA</param>
		/// <param name="pixelType">GL_UNSIGNED_BYTE</param>
		static Texture* Create(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

		static Texture* Create(const Image& image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

		/// <param name="shaderID">shaderProgramID</param>
		/// <param name="uniform">"tex0"</param>
		/// <param name="unit">0</param>
		virtual void SetTextureUnit(GLShader& /*shader*/, const char* /*uniform*/, GLuint /*unit*/) {}

		virtual uvec2 GetSize() const { return { 0,0 }; }
		u32 GetWidth() const { return GetSize().x; }
		u32 GetHeight() const { return GetSize().y; }
	};



	
}
