#include "rendering/batched_renderer2D.h"

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
			fvec2 TexCoord; //default is k_FullTextureCoords
			float TexIndex; //GPUs like floats, but essentially [0] to [31], but [0] is white.
			float TilingFactor; //default is 1
		};

		struct RenderData2D
		{
			static constexpr u32 k_MaxQuads = 2000;
			static constexpr u32 k_MaxVertices = k_MaxQuads * 4;
			static constexpr u32 k_MaxIndices = k_MaxQuads * 6;
			static constexpr u32 k_MaxTextureSlots = 32; //[0] is white texture, so 31 others

			std::shared_ptr<VertexArray> m_QuadVertexArray;
			std::shared_ptr<VertexBuffer> m_QuadVertexBuffer;
			u32 m_QuadIndexCount = 0;
			QuadVertex* m_QuadVertexBufferBase = nullptr;
			QuadVertex* m_QuadVertexBufferPtr = nullptr;
			GLShader* m_QuadShader = nullptr;
			glm::vec4 m_QuadVertexPositions[4];

			std::shared_ptr<Texture> m_WhiteTexture = nullptr;
			std::array<std::shared_ptr<Texture>, k_MaxTextureSlots> m_TextureSlots;
			u32 m_TextureSlotIndex = 1; //texture ID counter, [0] is white.

			std::unordered_map<u32, std::shared_ptr<Texture>> m_TextureIdMap; //Key is Image ID

			struct {
				u32 m_DrawCalls = 0;
				u32 m_QuadCount = 0;
			} m_Stats;
		};

		static RenderData2D s_Data;
	}

	/*static*/ void BatchedRenderer2D::Init()
	{
		s_Data.m_QuadVertexArray.reset(VertexArray::Create());

		//Quad Verts
		{
			s_Data.m_QuadVertexBuffer.reset(VertexBuffer::Create(s_Data.k_MaxVertices * sizeof(QuadVertex)));
			s_Data.m_QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     },
				{ ShaderDataType::Float,  "a_TilingFactor" },
				//{ ShaderDataType::Int,    "a_EntityID"     }
			});
			s_Data.m_QuadVertexArray->AddVertexBuffer(s_Data.m_QuadVertexBuffer);
			s_Data.m_QuadVertexBufferBase = new QuadVertex[s_Data.k_MaxVertices];
		}

		//Quad Indicies
		{
			u32* quadIndices = new u32[s_Data.k_MaxIndices];

			u32 offset = 0;
			for (u32 i = 0; i < s_Data.k_MaxIndices; i += 6)
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
		}

		//Textures
		{
			s_Data.m_WhiteTexture.reset(Texture::Create(1u, 1u));
			const u32 whiteTextureData = 0xffffffff;
			s_Data.m_WhiteTexture->SetData(&whiteTextureData, sizeof(u32));
			s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;
		}

		//Quad Shader
		{
			s_Data.m_QuadShader = &Renderer::s_shaderTextureBatchedTransform2D;

			s_Data.m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.m_QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.m_QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
		}
	}
	/*static*/ void BatchedRenderer2D::Shutdown()
	{
		delete[] s_Data.m_QuadVertexBufferBase;
	}

	/*static*/ void BatchedRenderer2D::BeginScene()
	{
		s_Data.m_Stats.m_QuadCount = 0;
		s_Data.m_Stats.m_DrawCalls = 0;

		s_Data.m_QuadShader->Bind();
		s_Data.m_QuadShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());

		StartBatch();
	}
	/*static*/ void BatchedRenderer2D::EndScene()
	{
		FlushBatch();

		std::cout << "Quad Count: " << s_Data.m_Stats.m_QuadCount << "  Draw Calls: " << s_Data.m_Stats.m_DrawCalls << "\r\n";
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
			const u32 dataSize = (u32)((u8*)s_Data.m_QuadVertexBufferPtr - (u8*)s_Data.m_QuadVertexBufferBase);
			s_Data.m_QuadVertexBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);

			// Bind textures
			for (u32 i = 0; i < s_Data.m_TextureSlotIndex; i++)
				s_Data.m_TextureSlots[i]->Bind(i);
		
			s_Data.m_QuadShader->Bind();
		
			//TODO: Look into passing vertex array to DrawIndexed() and binding within
			s_Data.m_QuadVertexArray->Bind();
			GetRendererAPI()->DrawIndexed(s_Data.m_QuadIndexCount);

			s_Data.m_Stats.m_DrawCalls++;
		}
	}
	/*static*/ void BatchedRenderer2D::NextBatch()
	{
		FlushBatch();
		StartBatch();
	}


	/*static*/ void BatchedRenderer2D::DrawQuad(const ftransform2D& transform, const fcolor4& color)
	{
		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		if (s_Data.m_QuadIndexCount >= RenderData2D::k_MaxIndices)
			NextBatch();

		for (size_t i = 0; i < k_FullTextureCoords.size(); i++)
		{
			s_Data.m_QuadVertexBufferPtr->Position = transform.MakeTransformMatrix() * s_Data.m_QuadVertexPositions[i];
			s_Data.m_QuadVertexBufferPtr->Color = color;
			s_Data.m_QuadVertexBufferPtr->TexCoord = k_FullTextureCoords[i];
			s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			//s_Data.m_QuadVertexBufferPtr->EntityID = entityID;
			s_Data.m_QuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.m_QuadCount++;
	}

	/*static*/ std::shared_ptr<Texture> BatchedRenderer2D::GetOrLoadTexture(const Image& image)
	{
		std::shared_ptr<Texture> t;
		if (s_Data.m_TextureIdMap.contains(image.GetId()))
		{
			return s_Data.m_TextureIdMap[image.GetId()];
		}
		else
		{
			t.reset(Texture::Create(image, GL_TEXTURE_2D, GL_TEXTURE0, (image.GetNumChannels() == 4) ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE));
			s_Data.m_TextureIdMap.insert({ image.GetId(), t });
			return t;
		}
	}

	/*static*/ void BatchedRenderer2D::DrawSubTexturedQuad(const ftransform2D& transform, const SubImage& subImage, float tilingFactor, const fcolor4& tintColor)
	{
		DrawSubTexturedQuad(transform, GetOrLoadTexture(*subImage.GetImage()), subImage.GetTexCoords(), tilingFactor, tintColor);
	}

	/*static*/ void BatchedRenderer2D::DrawSubTexturedQuad(const ftransform2D& transform, const std::shared_ptr<Texture>& texture, const std::array<fvec2, 4> textureCoords, float tilingFactor, const fcolor4& tintColor)
	{
		if (s_Data.m_QuadIndexCount >= RenderData2D::k_MaxIndices)
			NextBatch();

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.m_TextureSlotIndex; i++)
		{
			if (s_Data.m_TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.m_TextureSlotIndex >= RenderData2D::k_MaxTextureSlots)
				NextBatch();

			textureIndex = (float)s_Data.m_TextureSlotIndex;
			s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
			s_Data.m_TextureSlotIndex++;
		}

		for (size_t i = 0; i < textureCoords.size(); i++)
		{
			s_Data.m_QuadVertexBufferPtr->Position = transform.MakeTransformMatrix() * s_Data.m_QuadVertexPositions[i];
			s_Data.m_QuadVertexBufferPtr->Color = tintColor;
			s_Data.m_QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.m_QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			//s_Data.m_QuadVertexBufferPtr->EntityID = entityID;
			s_Data.m_QuadVertexBufferPtr++;
		}

		s_Data.m_QuadIndexCount += 6;

		s_Data.m_Stats.m_QuadCount++;
	}
}