#include "rendering_primitives.h"

namespace Poole::Rendering
{
	bool IMesh::UsesUniformColor() const { return dynamic_cast<const MeshUniform_SolidColorBase*>(this); }
	bool IMesh::Uses2DTransform() const { return dynamic_cast<const MeshUniformCollection_2DTransform*>(this); }
	void IMesh::SetUniforms(GLuint programID)
	{
		if (IMeshUniformCollectorBase* MeshUniformCollector = dynamic_cast<IMeshUniformCollectorBase*>(this))
		{
			MeshUniformCollector->SetAllUniforms(programID);
		}
	}


	void StaticMeshNoIndiciesSolidColor3::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);
	}
	void StaticMeshNoIndiciesSolidColor3::Render(GLuint programID)
	{
		//Use Shader
		glUseProgram(programID);

		SetUniforms(programID);
		//GLint Loc = glGetUniformLocation(programID, "uniformColor");
		//glUniform3f(Loc, m_color.r, m_color.g, m_color.b);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(Vertex) / sizeof(f32),   //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			3 * sizeof(f32),				//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_verts.size()); //Starting from vertex 0; N vertices total.
		glDisableVertexAttribArray(0);
	}

	//---

	void StaticMeshSolidColor3::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);
		
		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);
		
		//Generate 1 buffer, put the resulting identifier in m_elementbuffer
		glGenBuffers(1, &m_elementbuffer);
		//The following commands will talk about our 'm_elementbuffer' buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		//Give our indices to OpenGL.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	void StaticMeshSolidColor3::Render(GLuint programID)
	{
		//Use Shader
		glUseProgram(programID);

		SetUniforms(programID);
		//GLint Loc = glGetUniformLocation(programID, "uniformColor");
		//glUniform3f(Loc, m_color.r, m_color.g, m_color.b);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(Vertex) / sizeof(f32),   //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			0,								//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	}

	//--

	void StaticMeshVertexColor3::Init()
	{
		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexWithColor3) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);

		//Generate 1 buffer, put the resulting identifier in m_elementbuffer
		glGenBuffers(1, &m_elementbuffer);
		//The following commands will talk about our 'm_elementbuffer' buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	void StaticMeshVertexColor3::Render(GLuint programID)
	{
		//Use Shader
		glUseProgram(programID);

		SetUniforms(programID);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		//0th attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(Vertex) / sizeof(f32),   //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			sizeof(VertexWithColor3),		//stride
			(void*)0						//array buffer offset
		);

		//1st attribute buffer : color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,									//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(fcolor3) / sizeof(f32),		//size
			GL_FLOAT,							//type
			GL_FALSE,							//normalized?
			sizeof(VertexWithColor3),			//stride
			(char*)(sizeof(Vertex))				//array buffer offset
		);

		//Draw the triangle !
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
}
