#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class VertexArray;

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() {}
		static RendererAPI* GetOrCreate();

		virtual void SetClearColor(const fcolor4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(u32 count) = 0;
		virtual void DrawLines(const std::shared_ptr<VertexArray> vertexArray, uint32_t vertexCount) = 0;
	};

	static inline RendererAPI* GetRendererAPI() { return RendererAPI::GetOrCreate(); }
}
