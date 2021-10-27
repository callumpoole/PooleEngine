
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
	private:
		uint32_t m_RendererID;
	};
}
