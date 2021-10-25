#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		static VertexBuffer* Create(f32* verts, u32 size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual u32 GetCount() const = 0;
		static IndexBuffer* Create(u32* indices, u32 count);
	};
}
