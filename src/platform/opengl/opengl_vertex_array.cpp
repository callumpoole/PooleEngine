
#include "opengl_vertex_array.h"

namespace Poole::Rendering
{
	OpenGL_VertexArray::OpenGL_VertexArray()
	{
		//glCreateVertexArrays(1, &m_RendererID);
		glGenVertexArrays(1, &m_RendererID);
	}
	OpenGL_VertexArray::~OpenGL_VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGL_VertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGL_VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}
}
