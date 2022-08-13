#include "rendering/renderer2D.h"

#include "graphics_api/buffer.h"
#include "graphics_api/vertex_array.h"
#include "graphics_api/texture.h"
#include "poole/rendering/image/image.h"
#include "poole/rendering/image/sub_image.h"
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

		std::unordered_map<u32, std::shared_ptr<Texture>> m_TextureIdMap; //Key is Image ID

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
				-0.5, -0.5, 0.f,		0.f, 0.f,
				 0.5, -0.5, 0.f,		1.f, 0.f,
				 0.5,  0.5, 0.f,		1.f, 1.f,
				-0.5,  0.5, 0.f,		0.f, 1.f,
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
		SCOPED_PROFILER();

		s_QuadRenderData.m_VertexArray->Bind(); 

		m_QuadShader->Bind();
		m_QuadShader->SetUniform("u_Transform", transform.MakeTransformMatrix());
		m_QuadShader->SetUniform("u_Color", color);

		GetRendererAPI()->DrawIndexed(6);
	}

	void Renderer2D::DrawCircle(const ftransform2D& transform, const fcolor4& color, float thickness, float fade)
	{
		s_QuadRenderData.m_VertexArray->Bind();

		m_CircleShader->Bind();

		//It's cheaper to process in C++, than in each fragment
		const fmat4 Transform = transform.MakeTransformMatrix();
		const fmat4 CameraViewProj = Renderer::GetCamera().GetViewProjectionMatrix();
		m_CircleShader->SetUniform("u_InvTransform_InvCameraViewProj", glm::inverse(Transform) * glm::inverse(CameraViewProj));

		m_CircleShader->SetUniform("u_Color", color);
		m_CircleShader->SetUniform("u_Thickness", thickness);
		m_CircleShader->SetUniform("u_Fade", std::max(fade, 0.000001f));

		GetRendererAPI()->DrawIndexed(6);
	}


	

	/*static*/ std::shared_ptr<Texture> Renderer2D::GetOrLoadTexture(const Image& image)
	{
		std::shared_ptr<Texture> t;
		if (m_TextureIdMap.contains(image.GetId()))
		{
			return m_TextureIdMap[image.GetId()];
		}
		else
		{
			t.reset(Texture::Create(image, GL_TEXTURE_2D, GL_TEXTURE0, (image.GetNumChannels() == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE));
			m_TextureIdMap.insert({ image.GetId(), t });
			return t;
		}
	}
	void Renderer2D::DrawTexturedQuad(const ftransform2D& transform, const Image& image)
	{
		SCOPED_PROFILER();

		std::shared_ptr<Texture> texture = GetOrLoadTexture(image);

		s_TextureRenderData.m_VertexArray->Bind();

		m_TextureShader->Bind();
		m_TextureShader->SetUniform("u_Transform", transform.MakeTransformMatrix());
		m_TextureShader->SetUniform("u_Color", Colors::White<fcolor4>);
		texture->Bind();
		texture->SetTextureUnit(*m_TextureShader, "tex0", 0);

		GetRendererAPI()->DrawIndexed(6);

		texture->Unbind();
		s_TextureRenderData.m_VertexArray->Unbind();
	}
	void Renderer2D::DrawSubTexturedQuad(const ftransform2D& transform, const SubImage& subImage)
	{
		std::shared_ptr<Texture> texture = GetOrLoadTexture(*subImage.GetImage());

		s_TextureRenderData.m_VertexArray->Bind();

		m_TextureShader->Bind();
		m_TextureShader->SetUniform("u_Transform", transform.MakeTransformMatrix());
		m_TextureShader->SetUniform("u_Color", Colors::White<fcolor4>);
		texture->Bind();
		texture->SetTextureUnit(*m_TextureShader, "tex0", 0);


		GetRendererAPI()->DrawIndexed(6);

		texture->Unbind();
		s_TextureRenderData.m_VertexArray->Unbind();
	}
}
