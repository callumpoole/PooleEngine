
#include "vertex_array.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_texture.h"

namespace Poole::Rendering
{
	/*static*/ Texture* Texture::Create(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: assert(false); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_Texture(imagePath, texType, slot, format, pixelType);
		}
		assert(false);
		return nullptr;
	}

	/*static*/ Texture* Texture::Create(const Image& image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: assert(false); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_Texture(image, texType, slot, format, pixelType);
		}
		assert(false);
		return nullptr;
	}

	/*static*/ Texture* Texture::Create(u32 width, u32 height)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: assert(false); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_Texture(width, height);
		}
		assert(false);
		return nullptr;
	}
}
