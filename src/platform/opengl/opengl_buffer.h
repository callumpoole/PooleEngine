#pragma once

#include "poole/core.h"
#include "rendering/graphics_api/buffer.h"

namespace Poole::Rendering
{
	class OpenGL_VertexBuffer : public VertexBuffer
	{
	public:
		//Dynamic Draw
		OpenGL_VertexBuffer(u32 size);
		//Static Draw
		OpenGL_VertexBuffer(f32* verts, u32 size);
		virtual ~OpenGL_VertexBuffer();

		virtual void Bind();
		virtual void Unbind();

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const VertexBufferLayout& GetLayout() const override { return m_layout; }
		virtual void SetLayout(const VertexBufferLayout& layout) override { m_layout = layout; }
	private:
		u32 m_RendererID;
		VertexBufferLayout m_layout;
	};

	class OpenGL_IndexBuffer : public IndexBuffer
	{
	public:
		OpenGL_IndexBuffer(u32* indices, u32 count);
		virtual ~OpenGL_IndexBuffer();

		virtual void Bind();
		virtual void Unbind();
		virtual u32 GetCount() const { return m_Count; }
	private:
		u32 m_RendererID;
		u32 m_Count;
	};
}
