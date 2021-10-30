#include "opengl_renderer_api.h"

namespace Poole::Rendering
{
	void OpenGL_RendererAPI::DrawIndexed(u32 count)
	{
		glDrawElements(GL_TRIANGLES, (GLsizei)count, GL_UNSIGNED_INT, 0);
	}
}
