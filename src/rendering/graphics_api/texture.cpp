
#include "vertex_array.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_texture.h"

namespace Poole::Rendering
{
	/*static*/ Texture* Texture::Create(const char* imagePath, GLenum slot)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_Texture(imagePath, slot);
		}
		HALT();
		return nullptr;
	}

	/*static*/ Texture* Texture::Create(const Image& image, GLenum slot)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_Texture(image, slot);
		}
		HALT();
		return nullptr;
	}

	/*static*/ Texture* Texture::Create(uvec2 size)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_Texture(size);
		}
		HALT();
		return nullptr;
	}
}
