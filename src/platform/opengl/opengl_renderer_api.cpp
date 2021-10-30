#include "opengl_renderer_api.h"

namespace Poole::Rendering
{
	void OpenGL_RendererAPI::SetClearColor(const fcolor4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGL_RendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGL_RendererAPI::DrawIndexed(u32 count)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, 0);
	}
}
