#include "buffer.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_buffer.h"

namespace Poole::Rendering
{
	VertexBuffer* VertexBuffer::Create(f32* verts, u32 size)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: assert(false); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_VertexBuffer(verts, size);
		}

		assert(false);
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(u32* indices, u32 count)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: assert(false); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_IndexBuffer(indices, count);
		}

		assert(false);
		return nullptr;
	}
}