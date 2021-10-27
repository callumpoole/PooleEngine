#include "renderer_api.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_renderer_api.h"

namespace Poole::Rendering
{
	RendererAPI* RendererAPI::Get()
	{
		switch (Renderer::s_GraphicsAPI)
		{
			case EGraphicsAPI::None: assert(false); return nullptr;
			case EGraphicsAPI::OpenGL: 
			{
				static OpenGL_RendererAPI* OpenGL = new OpenGL_RendererAPI();
				return OpenGL;
			}
		}

		assert(false);
		return nullptr;
	}
}
