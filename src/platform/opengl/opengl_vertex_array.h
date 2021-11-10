
#pragma once

#include "core.h"
#include "rendering/graphics_api/vertex_array.h"

namespace Poole::Rendering
{
	class OpenGL_VertexArray : public VertexArray
	{
	public:
		OpenGL_VertexArray();
		~OpenGL_VertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}
