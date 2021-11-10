
#pragma once

#include "core.h"
#include "buffer.h"

namespace Poole::Rendering
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		static VertexArray* Create();
	};
}
