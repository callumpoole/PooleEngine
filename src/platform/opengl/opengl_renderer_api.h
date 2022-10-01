#pragma once

#include "core.h"
#include "rendering/graphics_api/renderer_api.h"

namespace Poole::Rendering
{
	class OpenGL_RendererAPI : public RendererAPI
	{
	public:
		OpenGL_RendererAPI() = default;

		virtual void SetClearColor(const fcolor4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(u32 count) override;

		virtual void DrawLines(const std::shared_ptr<VertexArray> vertexArray, uint32_t vertexCount) override;
	};
}
