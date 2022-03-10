#include "rendering/renderer2D.h"

#include "graphics_api/buffer.h"
#include "graphics_api/vertex_array.h"
#include "graphics_api/texture.h"
#include "rendering/renderer.h"
#include "rendering/graphics_api/renderer_api.h"
#include "rendering/camera/orthographic_camera.h"
#include "window/window.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Poole::Rendering
{
	namespace 
	{
		struct RenderData
		{
			std::shared_ptr<VertexArray> m_VertexArray;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
		};
		static RenderData s_QuadRenderData;
		static RenderData s_TextureRenderData;

		std::vector<std::shared_ptr<Texture>> m_Textures;

		GLShader* m_QuadShader;
		GLShader* m_CircleShader;
		GLShader* m_TextureShader;
	}

	void Renderer2D::Init()
	{
		//Shader
		m_QuadShader = &Renderer::s_shaderUniformColorTransform2D;
		m_CircleShader = &Renderer::s_shaderCircleTransform2D;
		m_TextureShader = &Renderer::s_shaderTextureTransform2D;

		u32 indices[6] = { 0, 1, 2, 2, 3, 0 };

		//Colored Quad
		{
			//Vertex Array
			s_QuadRenderData.m_VertexArray.reset(VertexArray::Create());
			s_QuadRenderData.m_VertexArray->Bind();

			//Vertex Buffer
			fvec3 corners[4] =
			{
				{-1, -1, 0.f},
				{ 1, -1, 0.f},
				{ 1,  1, 0.f},
				{-1,  1, 0.f},
			};
			s_QuadRenderData.m_VertexBuffer.reset(VertexBuffer::Create((f32*)corners, sizeof(corners)));
			s_QuadRenderData.m_VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"},
			});
			s_QuadRenderData.m_VertexArray->AddVertexBuffer(s_QuadRenderData.m_VertexBuffer);

			//Index Buffer
			s_QuadRenderData.m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(u32)));
			s_QuadRenderData.m_VertexArray->SetIndexBuffer(s_QuadRenderData.m_IndexBuffer);
		}


		//Textured Quad
		{
			//Vertex Array (Texture)
			s_TextureRenderData.m_VertexArray.reset(VertexArray::Create());
			s_TextureRenderData.m_VertexArray->Bind();

			//Vertex Buffer (Texture)
			f32 cornersForTexture[] =
			{
				-1, -1, 0.f,		0.f, 0.f,
				 1, -1, 0.f,		1.f, 0.f,
				 1,  1, 0.f,		1.f, 1.f,
				-1,  1, 0.f,		0.f, 1.f,
			};
			s_TextureRenderData.m_VertexBuffer.reset(VertexBuffer::Create((f32*)cornersForTexture, sizeof(cornersForTexture)));
			s_TextureRenderData.m_VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float2, "a_Texture"},
			});
			s_TextureRenderData.m_VertexArray->AddVertexBuffer(s_TextureRenderData.m_VertexBuffer);

			//Index Buffer (Texture)
			s_TextureRenderData.m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(u32)));
			s_TextureRenderData.m_VertexArray->SetIndexBuffer(s_TextureRenderData.m_IndexBuffer);
		}
	}
	void Renderer2D::Shutdown()
	{

	}
	void Renderer2D::BeginScene()
	{
		//Colored Quad
		{
			//Vertex Array
			s_QuadRenderData.m_VertexArray->Bind();
			//Vertex Buffer
			s_QuadRenderData.m_VertexBuffer->Bind();
			//Index Buffer
			s_QuadRenderData.m_IndexBuffer->Bind();
		
			//Shader
			m_QuadShader->Bind();
			m_QuadShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());
		}
		
		//Textured Quad
		{
			//Vertex Array
			s_TextureRenderData.m_VertexArray->Bind();
			//Vertex Buffer
			s_TextureRenderData.m_VertexBuffer->Bind();
			//Index Buffer
			s_TextureRenderData.m_IndexBuffer->Bind();
		
			m_TextureShader->Bind();
			m_TextureShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());
		}
		
		//Circles
		{
			m_CircleShader->Bind();
			m_CircleShader->SetUniform("u_WindowSize", (fvec2)Window::GetWindowSize());
		}
	}
	void Renderer2D::RenderScene()
	{

	}
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const ftransform2D& transform, const fcolor4& color)
	{
		s_QuadRenderData.m_VertexArray->Bind(); 

		m_QuadShader->Bind();
		m_QuadShader->SetUniform("u_Transform", MakeTransformMatrix(transform));
		m_QuadShader->SetUniform("u_Color", color);

		s_QuadRenderData.m_VertexBuffer->Bind(); //Probably unnecessary
		s_QuadRenderData.m_IndexBuffer->Bind();	 //Probably unnecessary

		GetRendererAPI()->DrawIndexed(6);
	}

	void Renderer2D::DrawCircle(const ftransform2D& transform, const fcolor4& color, float thickness, float fade)
	{
		s_QuadRenderData.m_VertexArray->Bind();

		m_CircleShader->Bind();

		//It's cheaper to process in C++, than in each fragment
		const fmat4 Transform = MakeTransformMatrix(transform);
		const fmat4 CameraViewProj = Renderer::GetCamera().GetViewProjectionMatrix();
		m_CircleShader->SetUniform("u_InvTransform_InvCameraViewProj", glm::inverse(Transform) * glm::inverse(CameraViewProj));

		m_CircleShader->SetUniform("u_Color", color);
		m_CircleShader->SetUniform("u_Thickness", thickness);
		m_CircleShader->SetUniform("u_Fade", std::max(fade, 0.000001f));

		s_QuadRenderData.m_VertexBuffer->Bind(); //Probably unnecessary
		s_QuadRenderData.m_IndexBuffer->Bind();	 //Probably unnecessary

		GetRendererAPI()->DrawIndexed(6);
	}


	

	fmat4 Renderer2D::MakeTransformMatrix(const ftransform2D& transform)
	{
		//TODO: Optimise, can probably only modify the first 2 rows/columns perhaps
		const fmat4 shearMat = {
							1, transform.shear.y, 0, 0,
			transform.shear.x,				   1, 0, 0,
							0,				   0, 1, 0,
							0,				   0, 0, 1,
		};

		//TODO: Optimise, put one func inside the next instead of the identity fmat4
		return
			glm::translate(fmat4(1.0f), transform.position) *
			glm::rotate(fmat4(1.0f), transform.rotation, fvec3{0,0,1}) *
			shearMat *
			glm::scale(fmat4(1.0f), fvec3{ transform.scale.x, transform.scale.y, 1.f });
	}


	TextureHandle Renderer2D::LoadTexture(const char* path, bool hasAlpha)
	{
		const TextureHandle outHandle = m_Textures.size();

		std::shared_ptr<Texture> t;
		t.reset(Texture::Create(path, GL_TEXTURE_2D, GL_TEXTURE0, hasAlpha ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE));
		m_Textures.push_back(std::move(t));
		return outHandle;
	}
	void Renderer2D::DrawTexturedQuad(const ftransform2D& transform, TextureHandle handle)
	{
		if (handle >= 0 && handle < m_Textures.size())
		{
			s_TextureRenderData.m_VertexArray->Bind();

			m_TextureShader->Bind();
			m_TextureShader->SetUniform("u_Transform", MakeTransformMatrix(transform));
			m_TextureShader->SetUniform("u_Color", Colors::White<fcolor4>);
			m_Textures[handle]->Bind();
			m_Textures[handle]->SetTextureUnit(*m_TextureShader, "tex0", 0);

			s_TextureRenderData.m_VertexBuffer->Bind(); //Probably unnecessary
			s_TextureRenderData.m_IndexBuffer->Bind();	 //Probably unnecessary

			GetRendererAPI()->DrawIndexed(6);

			m_Textures[handle]->Unbind();
		}
		else
		{
			LOG_ERROR("invalid texture handle {}", handle);
		}
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
