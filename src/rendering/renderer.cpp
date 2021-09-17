
#include "renderer.h"
#include "core.h"

namespace Poole::Rendering
{
	std::vector<std::unique_ptr<MeshBasicSolidColor>> Renderer::m_solidColorMeshes;
	std::vector<std::unique_ptr<MeshBasicVertexColor>> Renderer::m_vertexColorMeshes;
	GLuint Renderer::m_vertexbuffer;

	MeshBasicSolidColor* Renderer::Submit(const MeshBasicSolidColor& meshAndColor)
	{
		m_solidColorMeshes.emplace_back(std::make_unique<MeshBasicSolidColor>(meshAndColor));
		return InitLastSumbitted();
	}
	MeshBasicSolidColor* Renderer::Submit(MeshBasicSolidColor&& meshAndColor)
	{
		m_solidColorMeshes.emplace_back(std::make_unique<MeshBasicSolidColor>(std::move(meshAndColor)));
		return InitLastSumbitted();
	}
	//MeshBasicVertexColor* Renderer::Submit(const MeshBasicVertexColor& m_mesh)
	//{
	//	m_vertexColorMeshes.emplace_back(std::make_unique<MeshBasicVertexColor>(m_mesh));
	//	return m_vertexColorMeshes.back().get();
	//}
	//MeshBasicVertexColor* Renderer::Submit(MeshBasicVertexColor&& m_mesh)
	//{
	//	m_vertexColorMeshes.emplace_back(std::make_unique<MeshBasicVertexColor>(std::move(m_mesh)));
	//	return m_vertexColorMeshes.back().get();
	//}

	void Renderer::RenderAll()
	{
		for (std::unique_ptr<MeshBasicSolidColor>& ptr : m_solidColorMeshes)
		{
			RenderObject(ptr.get());
		}
	}

	MeshBasicSolidColor* Renderer::InitLastSumbitted()
	{
		MeshBasicSolidColor* ptr = m_solidColorMeshes.back().get();

		GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
    
        //Generate 1 buffer, put the resulting identifier in m_vertexbuffer
        glGenBuffers(1, &ptr->m_mesh.m_vertexbuffer);
        //The following commands will talk about our 'm_vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, ptr->m_mesh.m_vertexbuffer);
        //Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * ptr->m_mesh.m_verts.size(), ptr->m_mesh.m_verts.data(), GL_STATIC_DRAW);

		return ptr;
	}

	void Renderer::RenderObject(MeshBasicSolidColor* meshAndColor)
	{
		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, meshAndColor->m_mesh.m_vertexbuffer);
		glVertexAttribPointer(
			0,								//attribute 0. No particular reason for 0, but must match the layout in the shader.
			sizeof(Vertex)/sizeof(f32),     //size
			GL_FLOAT,						//type
			GL_FALSE,						//normalized?
			0,								//stride
			(void*)0						//array buffer offset
		);
		//Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)meshAndColor->m_mesh.m_verts.size()); //Starting from vertex 0; N vertices total.
		glDisableVertexAttribArray(0);
	}
}