#include "buffer.h"
#include "rendering/renderer.h"
#include "platform/opengl/opengl_buffer.h"

namespace Poole::Rendering
{
	VertexBuffer* VertexBuffer::Create(u32 size)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_VertexBuffer(size);
		}
		HALT();
		return nullptr;
	}

	VertexBuffer* VertexBuffer::Create(f32* verts, u32 size)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_VertexBuffer(verts, size);
		}
		HALT();
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(const u32* indices, u32 count)
	{
		switch (Renderer::s_GraphicsAPI)
		{
		case EGraphicsAPI::None: HALT(); return nullptr;
		case EGraphicsAPI::OpenGL: return new OpenGL_IndexBuffer(indices, count);
		}
		HALT();
		return nullptr;
	}
}