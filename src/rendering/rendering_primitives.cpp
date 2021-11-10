#include "poole/rendering/rendering_primitives.h"
#include "poole/rendering/renderer.h"

namespace Poole::Rendering
{
	void IMeshBase::SetUniforms(Shader& shader)
	{
		shader.SetUniform("u_CameraViewProjection", Poole::Rendering::Renderer::GetCamera().GetViewProjectionMatrix());
	}

	//--

	void StaticMeshNoIndiciesSolidColor4_2DTransform::Init()
	{
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);
	}
	void StaticMeshNoIndiciesSolidColor4_2DTransform::Render(Shader& shader)
	{
		glBindVertexArray(m_vertexArrayID);

		//Use Shader
		shader.Bind();
		SetUniforms(shader);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex) / sizeof(f32),  //count
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			sizeof(TVertex),				//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_verts.size()); //Starting from vertex 0; N vertices total.
		glDisableVertexAttribArray(0);
	}


	//---

	void StaticMeshSolidColor4_2DTransform::Init()
	{
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);

		//Generate 1 buffer, put the resulting identifier in m_elementbuffer
		glGenBuffers(1, &m_elementbuffer);
		//The following commands will talk about our 'm_elementbuffer' buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		//Give our indices to OpenGL.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	void StaticMeshSolidColor4_2DTransform::Render(Shader& shader)
	{
		glBindVertexArray(m_vertexArrayID);

		//Use Shader
		shader.Bind();
		SetUniforms(shader);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex) / sizeof(f32),  //count
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			sizeof(TVertex),				//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
	}

	//--

	void StaticMeshVertexColor4_2DTransform::Init()
	{
		glGenVertexArrays(1, &m_vertexArrayID);
		glBindVertexArray(m_vertexArrayID);

		//Generate 1 buffer, put the resulting identifier in m_vertexbuffer
		glGenBuffers(1, &m_vertexbuffer);
		//The following commands will talk about our 'm_vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * m_verts.size(), m_verts.data(), GL_STATIC_DRAW);

		//Generate 1 buffer, put the resulting identifier in m_elementbuffer
		glGenBuffers(1, &m_elementbuffer);
		//The following commands will talk about our 'm_elementbuffer' buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
		//Give our vertices to OpenGL.
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);
	}
	void StaticMeshVertexColor4_2DTransform::Render(Shader& shader)
	{
		glBindVertexArray(m_vertexArrayID);

		//Use Shader
		shader.Bind();
		SetUniforms(shader);

		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);

		//0th attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,										//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex::TVertex) / sizeof(f32), //count
			GL_FLOAT,								//type
			GL_FALSE,								//normalized?
			sizeof(TVertex),						//stride
			(void*)0								//array buffer offset
		);

		//1st attribute buffer : color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,										//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(TVertex::TColor) / sizeof(f32),	//count
			GL_FLOAT,								//type
			GL_FALSE,								//normalized?
			sizeof(TVertex),						//stride
			(char*)(sizeof(TVertex::TVertex))		//array buffer offset
		);

		//Draw the triangle !
		glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}




}
