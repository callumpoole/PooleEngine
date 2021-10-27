#include "rendering/renderer2D.h"

#include "graphics_api/buffer.h"
#include "graphics_api/vertex_array.h"
#include "rendering/renderer.h"
#include "rendering/camera/orthographic_camera.h"

namespace Poole::Rendering
{
	Renderer2D::RenderData Renderer2D::s_RenderData{};

	void Renderer2D::Init()
	{
		s_RenderData.m_Shader = &Renderer::s_shaderUniformColorTransform2D;

		s_RenderData.m_VertexArray = VertexArray::Create();
		s_RenderData.m_VertexArray->Bind();

		fvec3 corners[4] =
		{
			{-1, -1, 0.f},
			{ 1, -1, 0.f},
			{ 1,  1, 0.f},
			{-1,  1, 0.f},
		};
		s_RenderData.m_VertexBuffer = VertexBuffer::Create((f32*)corners, sizeof(corners));

		u32 indices[6] = { 0, 1, 2, 2, 3, 0 };
		s_RenderData.m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(u32));
	}
	void Renderer2D::Shutdown()
	{
		//Todo: Automate with unique_ptr (issue is it's private to engine currently)
		delete s_RenderData.m_VertexArray;
		delete s_RenderData.m_VertexBuffer;
		delete s_RenderData.m_IndexBuffer;
	}
	void Renderer2D::BeginScene()
	{
		s_RenderData.m_VertexArray->Bind();
		s_RenderData.m_Shader->Bind();
		s_RenderData.m_Shader->SetUniform("u_cameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());
	}
	void Renderer2D::RenderScene()
	{
		s_RenderData.m_VertexBuffer->Bind();
		s_RenderData.m_IndexBuffer->Bind();

		//1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,			//attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,			//count
			GL_FLOAT,	//type
			GL_FALSE,	//normalized?
			0,			//stride
			(void*)0	//array buffer offset
		);
		//Draw the triangle !
		DrawQuad({ 0, 0 }, { 0.1f, 0.1f }, Colors::Red<fcolor4>);
		glDrawElements(GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_INT, 0);
	}
	void Renderer2D::EndScene()
	{
		glDisableVertexAttribArray(0);
	}

	void Renderer2D::DrawQuad(const fvec2& pos, const fvec2& scale, const fcolor4& color)
	{
		s_RenderData.m_Shader->SetUniform("u_Transform", MakeTransformMatrix(pos, scale));
		s_RenderData.m_Shader->SetUniform("u_Color", color);
	}

	fmat3 Renderer2D::MakeTransformMatrix(const fvec2& pos, const fvec2& scale, f32 rotation, const fvec2& shear)
	{
		const fmat3 Translation = {
			1, 0, pos.x,
			0, 1, pos.y,
			0, 0, 1
		};
		const fmat3 Rotation = {
			cosf(rotation), -sinf(rotation), 0,
			sinf(rotation),  cosf(rotation), 0,
						 0,				  0, 1,
		};
		const fmat3 Scale = {
			scale.x,	   0, 0,
				  0, scale.y, 0,
				  0,	   0, 1
		};
		const fmat3 Shear = {
			      1, shear.x, 0,
			shear.y,       1, 0,
			      0,       0, 1
		};
		return Translation * Rotation * Shear * Scale;
	}

	//void Renderer2D::DrawTriangle(fvec3 p1, fvec3 p2, fvec3 p3, fcolor4 color)
	//{
	//
	//}
	//void Renderer2D::DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor4 color, f32 zLayer)
	//{
	//	DrawTriangle(fvec3{ p1.x, p1.y, zLayer }, fvec3{ p2.x, p2.y, zLayer }, fvec3{ p3.x, p3.y, zLayer }, color);
	//}
	//void Renderer2D::DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor3 color, f32 zLayer)
	//{
	//	DrawTriangle(p1, p2, p3, fcolor4(color, 1.f), zLayer);
	//}
	//void Renderer2D::DrawTriangle(Vertex3Color4 p1, Vertex3Color4 p2, Vertex3Color4 p3)
	//{
	//
	//}
	//void Renderer2D::DrawTriangle(Vertex3Color3 p1, Vertex3Color3 p2, Vertex3Color3 p3)
	//{
	//	DrawTriangle(ToVertex3Color4(p1), ToVertex3Color4(p2), ToVertex3Color4(p3));
	//}
	//void Renderer2D::DrawTriangle(Vertex2Color4 p1, Vertex2Color4 p2, Vertex2Color4 p3, f32 zLayer)
	//{
	//	DrawTriangle(ToVertex3Color4(p1, zLayer), ToVertex3Color4(p2, zLayer), ToVertex3Color4(p3, zLayer));
	//}
	//void Renderer2D::DrawTriangle(Vertex2Color3 p1, Vertex2Color3 p2, Vertex2Color3 p3, f32 zLayer)
	//{
	//	DrawTriangle(ToVertex3Color4(p1, zLayer), ToVertex3Color4(p2, zLayer), ToVertex3Color4(p3, zLayer));
	//}
}
