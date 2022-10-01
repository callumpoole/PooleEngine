#include "renderer_api.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_renderer_api.h"

namespace Poole::Rendering
{
	RendererAPI* RendererAPI::GetOrCreate()
	{
		switch (Renderer::s_GraphicsAPI)
		{
			case EGraphicsAPI::None: HALT(); return nullptr;
			case EGraphicsAPI::OpenGL: 
			{
				static OpenGL_RendererAPI* OpenGL = new OpenGL_RendererAPI();
				return OpenGL;
			}
		}
		HALT();
		return nullptr;
	}
}
