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
		static RenderData s_CircleRenderData;
		static RenderData s_TextureRenderData;

		std::unordered_map<u32, std::shared_ptr<Texture>> m_TextureIdMap; //Key is Image ID

		GLShader* m_QuadShader;
		GLShader* m_CircleShader;
		GLShader* m_TextureShader;


		static constexpr u32 k_CornersOnAQuad = 4;
		static constexpr fvec3 k_CornersForQuad[k_CornersOnAQuad] =
		{
			{-0.5, -0.5, 0.f},
			{ 0.5, -0.5, 0.f},
			{ 0.5,  0.5, 0.f},
			{-0.5,  0.5, 0.f},
		};
		static constexpr fvec3 k_CornersForCircle[k_CornersOnAQuad] =
		{
			{-1, -1, 0.f},
			{ 1, -1, 0.f},
			{ 1,  1, 0.f},
			{-1,  1, 0.f},
		};
		struct TexturedVertex { fvec3 pos; fvec2 uv; };
		static constexpr TexturedVertex k_CornersForTexture[k_CornersOnAQuad] =
		{
			fvec3{-0.5, -0.5, 0.f},	fvec2{0.f, 0.f},
			fvec3{ 0.5, -0.5, 0.f},	fvec2{1.f, 0.f},
			fvec3{ 0.5,  0.5, 0.f},	fvec2{1.f, 1.f},
			fvec3{-0.5,  0.5, 0.f},	fvec2{0.f, 1.f},
		};
		////Will be modified, UVs can be overwritten
		static TexturedVertex s_CornersForSubTexture[k_CornersOnAQuad] =
		{
			fvec3{-0.5, -0.5, 0.f},	fvec2{0.f, 0.f},
			fvec3{ 0.5, -0.5, 0.f},	fvec2{1.f, 0.f},
			fvec3{ 0.5,  0.5, 0.f},	fvec2{1.f, 1.f},
			fvec3{-0.5,  0.5, 0.f},	fvec2{0.f, 1.f},
		};
	}

	void Renderer2D::Init()
	{
		//Shader
		m_QuadShader = &Renderer::s_shaderUniformColorTransform2D;
		m_CircleShader = &Renderer::s_shaderCircleTransform2D;
		m_TextureShader = &Renderer::s_shaderTextureTransform2D;

		constexpr u32 indices[6] = { 0, 1, 2, 2, 3, 0 };

		auto SetupData = [indices](RenderData& data, const f32* corners, u32 cornersSize, VertexBufferLayout&& VBLayout)
		{
			//Vertex Array
			data.m_VertexArray.reset(VertexArray::Create());
			data.m_VertexArray->Bind();

			//Vertex Buffer
			data.m_VertexBuffer.reset(VertexBuffer::Create((f32*)corners, cornersSize));
			data.m_VertexBuffer->SetLayout(VBLayout);
			data.m_VertexArray->AddVertexBuffer(data.m_VertexBuffer);

			//Index Buffer
			data.m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(u32)));
			data.m_VertexArray->SetIndexBuffer(data.m_IndexBuffer);
		};

		//Quad
		SetupData(s_QuadRenderData, (f32*)k_CornersForQuad, sizeof(k_CornersForQuad), 
			VertexBufferLayout{
				{ ShaderDataType::Float3, "a_Position"},
			});

		//Textured Quad
		SetupData(s_TextureRenderData, (f32*)k_CornersForTexture, sizeof(k_CornersForTexture),
			VertexBufferLayout{
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float2, "a_Texture"},
			});

		//Circle
		SetupData(s_CircleRenderData, (f32*)k_CornersForCircle, sizeof(k_CornersForCircle),
			VertexBufferLayout{
				{ ShaderDataType::Float3, "a_Position"},
			});
	}
	void Renderer2D::Shutdown()
	{

	}
	void Renderer2D::BeginScene()
	{
		auto Begin = [](RenderData& Data)
		{
			//Vertex Array
			Data.m_VertexArray->Bind();
			//Vertex Buffer
			Data.m_VertexBuffer->Bind();
			//Index Buffer
			Data.m_IndexBuffer->Bind();
		};

		//Colored Quad
		Begin(s_QuadRenderData);
		m_QuadShader->Bind();
		m_QuadShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());

		//Textured Quad
		Begin(s_TextureRenderData);
		m_TextureShader->Bind();
		m_TextureShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());
		
		//Circles
		Begin(s_CircleRenderData);
		m_CircleShader->Bind();
		m_CircleShader->SetUniform("u_WindowSize", (fvec2)Window::GetWindowSize());
		
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
		s_CircleRenderData.m_VertexArray->Bind();

		m_CircleShader->Bind();

		//It's cheaper to process in C++, than in each fragment
		const fmat4 Transform = glm::scale(transform.MakeTransformMatrix(), fvec3{ 0.5f, 0.5f, 1.f });
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
		s_TextureRenderData.m_VertexBuffer->Bind();
		s_TextureRenderData.m_VertexBuffer->SetData(k_CornersForTexture, sizeof(k_CornersForTexture));

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

		const std::array<fvec2, k_CornersOnAQuad>& coords = subImage.GetTexCoords();
		for (u32 i = 0; i < k_CornersOnAQuad; i++)
		{
			s_CornersForSubTexture[i].uv.x = coords[i].x;
			s_CornersForSubTexture[i].uv.y = coords[i].y;
		}
		
		s_TextureRenderData.m_VertexBuffer->Bind();
		s_TextureRenderData.m_VertexBuffer->SetData(s_CornersForSubTexture, sizeof(s_CornersForSubTexture));

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
