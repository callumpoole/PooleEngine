
#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class Image;
	class GLShader;

	class Texture
	{
		//===BOTH===
	public:
		virtual ~Texture() {}

		virtual uvec2 GetSize() const { return { 0,0 }; }
		u32 GetWidth() const { return GetSize().x; }
		u32 GetHeight() const { return GetSize().y; }
		
		//===OLD===
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		/// <param name="image">Path</param>
		/// <param name="slot">GL_TEXTURE0</param>
		static Texture* Create(const char* imagePath, GLenum slot);

		static Texture* Create(const Image& image, GLenum slot);

		/// <param name="shaderID">shaderProgramID</param>
		/// <param name="uniform">"tex0"</param>
		/// <param name="unit">0</param>
		virtual void SetTextureUnit(GLShader& /*shader*/, const char* /*uniform*/, GLuint /*unit*/) {}


		//===NEW===
	public:
		static Texture* Create(uvec2 size);
		virtual void SetData(const void* data, u32 size) = 0;
		virtual void Bind(u32 slot) = 0;
		virtual void Unbind(u32 slot) = 0;
	};
}
