
#include "vertex_array.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_vertex_array.h"

namespace Poole::Rendering
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_VertexArray();
		}
		HALT()
		return nullptr;
	}
}
