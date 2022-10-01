#include "opengl_renderer_api.h"

#include "rendering\graphics_api\vertex_array.h"

namespace Poole::Rendering
{
	void OpenGL_RendererAPI::SetClearColor(const fcolor4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGL_RendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO: Move to init
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}

	void OpenGL_RendererAPI::DrawIndexed(u32 count)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, 0);
	}

	void OpenGL_RendererAPI::DrawLines(const std::shared_ptr<VertexArray> vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}
}
