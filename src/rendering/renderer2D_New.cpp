#include "rendering/renderer2D_New.h"

#include "graphics_api/buffer.h"
#include "graphics_api/vertex_array.h"
#include "graphics_api/texture.h"
#include "poole/rendering/image/image.h"
#include "poole/rendering/image/sub_image.h"
#include "rendering/image/image.h"
#include "rendering/renderer.h"
#include "rendering/graphics_api/renderer_api.h"
#include "rendering/camera/orthographic_camera.h"
#include "window/window.h"

#include "glm/gtc/matrix_transform.hpp"

//Heavily inspired / copied from: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Renderer/Renderer2D.cpp

namespace Poole::Rendering
{
	namespace
	{
		struct QuadVertex
		{
			fvec3 Position;
			fvec4 Color;
			fvec2 TexCoord;
			float TexIndex; //GPUs like floats
		};

		struct RenderData2D
		{
			static constexpr u32 k_MaxQuads = 20000;
			static constexpr u32 k_MaxVertices = k_MaxQuads * 4;
			static constexpr u32 k_MaxIndices = k_MaxQuads * 6;
			static constexpr u32 k_MaxTextureSlots = 32; //[0] is white texture, so 31 others

			std::shared_ptr<VertexArray> m_QuadVertexArray;
			std::shared_ptr<VertexBuffer> m_QuadVertexBuffer;
			u32 m_QuadIndexCount = 0;
			QuadVertex* m_QuadVertexBufferBase = nullptr;
			QuadVertex* m_QuadVertexBufferPtr = nullptr;
			GLShader* m_QuadShader = nullptr;

			std::shared_ptr<Texture> m_WhiteTexture = nullptr;
			std::array<std::shared_ptr<Texture>, k_MaxTextureSlots> m_TextureSlots;
			u32 m_TextureSlotIndex = 1; //texture ID counter, [0] is white.
		};

		static RenderData2D s_Data;
	}

	/*static*/ void BatchedRenderer2D::Init()
	{
		s_Data.m_QuadVertexArray.reset(VertexArray::Create());

		//Quad Verts

		s_Data.m_QuadVertexBuffer.reset(VertexBuffer::Create(s_Data.k_MaxQuads * sizeof(QuadVertex)));
		s_Data.m_QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			//{ ShaderDataType::Float,  "a_TilingFactor" },
			//{ ShaderDataType::Int,    "a_EntityID"     }
		});
		s_Data.m_QuadVertexArray->AddVertexBuffer(s_Data.m_QuadVertexBuffer);
		s_Data.m_QuadVertexBufferBase = new QuadVertex[s_Data.k_MaxQuads];

		//Quad Indicies

		uint32_t* quadIndices = new uint32_t[s_Data.k_MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.k_MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> quadIB;
		quadIB.reset(IndexBuffer::Create(quadIndices, s_Data.k_MaxIndices));
		s_Data.m_QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		//Textures
		s_Data.m_WhiteTexture.reset(Texture::Create(Image::s_White1x1rgba, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE));
		s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;

		//Quad Shader
		s_Data.m_QuadShader = &Renderer::s_shaderTextureBatchedTransform2D;


	}
	/*static*/ void BatchedRenderer2D::Shutdown()
	{
		delete[] s_Data.m_QuadVertexBufferBase;
	}

	/*static*/ void BatchedRenderer2D::BeginScene()
	{
		s_Data.m_QuadShader->Bind();
		s_Data.m_QuadShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());

		StartBatch();
	}
	/*static*/ void BatchedRenderer2D::EndScene()
	{
		FlushBatch();
	}

	/*static*/ void BatchedRenderer2D::StartBatch()
	{
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;
	}
	/*static*/ void BatchedRenderer2D::FlushBatch()
	{
		if (s_Data.m_QuadIndexCount)
		{
			u32 dataSize = (u32)((u8*)s_Data.m_QuadVertexBufferPtr - (u8*)s_Data.m_QuadVertexBufferBase);
			s_Data.m_QuadVertexBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);

			// Bind textures
			for (u32 i = 0; i < s_Data.m_TextureSlotIndex; i++)
				s_Data.m_TextureSlots[i]->Bind(i);
		
			s_Data.m_QuadShader->Bind();
		
			//TODO: Look into passing vertex array to DrawIndexed() and binding within
			s_Data.m_QuadVertexArray->Bind();
			GetRendererAPI()->DrawIndexed(s_Data.m_QuadIndexCount);
			//s_Data.Stats.DrawCalls++;
		}
	}
	/*static*/ void BatchedRenderer2D::NextBatch()
	{
		FlushBatch();
		StartBatch();
	}

}