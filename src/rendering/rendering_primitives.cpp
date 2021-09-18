#include "rendering_primitives.h"

namespace Poole::Rendering
{
	void MeshBasicNoIndiciesSolidColor3::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_mesh.m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh.m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_mesh.m_verts.size(), m_mesh.m_verts.data(), GL_STATIC_DRAW);
	}
	void MeshBasicNoIndiciesSolidColor3::Render()
	{
		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh.m_vertexbuffer);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(Vertex) / sizeof(f32),     //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			0,								//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_mesh.m_verts.size()); //Starting from vertex 0; N vertices total.
		glDisableVertexAttribArray(0);
	}

	//---

	void MeshBasicSolidColor3::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_mesh.m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh.m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_mesh.m_verts.size(), m_mesh.m_verts.data(), GL_STATIC_DRAW);
	}
	void MeshBasicSolidColor3::Render()
	{
		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_mesh.m_vertexbuffer);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(Vertex) / sizeof(f32),     //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			0,								//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_mesh.m_verts.size()); //Starting from vertex 0; N vertices total.
		glDisableVertexAttribArray(0);
	}
}
