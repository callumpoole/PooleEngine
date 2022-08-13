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

		struct CircleVertex
		{
			fvec3 Position;
			fvec4 Color;
		};

		struct RenderData2D
		{
			static constexpr u32 k_MaxQuads = 2000;
			static constexpr u32 k_MaxVertices = k_MaxQuads * 4;
			static constexpr u32 k_MaxIndices = k_MaxQuads * 6;
			static constexpr u32 k_MaxTextureSlots = 32; //[0] is white texture, so 31 others
			static constexpr std::array<fvec4, 4> m_QuadAndCircleVertexPositions = {
				fvec4{ -0.5f, -0.5f, 0.0f, 1.0f },
				fvec4{ 0.5f, -0.5f, 0.0f, 1.0f },
				fvec4{ 0.5f,  0.5f, 0.0f, 1.0f },
				fvec4{ -0.5f,  0.5f, 0.0f, 1.0f }
			};

			//Quad Stuff
			std::shared_ptr<VertexArray> m_QuadVertexArray;
			std::shared_ptr<VertexBuffer> m_QuadVertexBuffer;
			u32 m_QuadIndexCount = 0;
			QuadVertex* m_QuadVertexBufferBase = nullptr;
			QuadVertex* m_QuadVertexBufferPtr = nullptr;
			GLShader* m_QuadShader = nullptr;

			//Circle Stuff
			std::shared_ptr<VertexArray> m_CircleVertexArray;
			std::shared_ptr<VertexBuffer> m_CircleVertexBuffer;
			u32 m_CircleIndexCount = 0;
			CircleVertex* m_CircleVertexBufferBase = nullptr;
			CircleVertex* m_CircleVertexBufferPtr = nullptr;
			GLShader* m_CircleShader = nullptr;

			//Texture Stuff
			std::shared_ptr<Texture> m_WhiteTexture = nullptr;
			std::array<std::shared_ptr<Texture>, k_MaxTextureSlots> m_TextureSlots;
			u32 m_TextureSlotIndex = 1; //texture ID counter, [0] is white.
			std::unordered_map<u32, std::shared_ptr<Texture>> m_TextureIdMap; //Key is Image ID

			//Stats
			struct {
				u32 m_DrawCalls = 0;
				u32 m_QuadCount = 0;
				u32 m_CircleCount = 0;
			} m_Stats;
		};

		static RenderData2D s_Data;
	}

	/*static*/ void BatchedRenderer2D::Init()
	{
		//Quad Vertex Array
		s_Data.m_QuadVertexArray.reset(VertexArray::Create());
		s_Data.m_QuadShader = &Renderer::s_shaderTextureBatchedTransform2D;

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


		//Circle Vertex Array
		s_Data.m_CircleVertexArray.reset(VertexArray::Create());
		s_Data.m_CircleShader = &Renderer::s_shaderCircleBatchedTransform2D;

		//Circle Verts
		{
			s_Data.m_CircleVertexBuffer.reset(VertexBuffer::Create(s_Data.k_MaxVertices * sizeof(CircleVertex)));
			s_Data.m_CircleVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				//{ ShaderDataType::Int,    "a_EntityID"     }
				});
			s_Data.m_CircleVertexArray->AddVertexBuffer(s_Data.m_CircleVertexBuffer);
			s_Data.m_CircleVertexBufferBase = new CircleVertex[s_Data.k_MaxVertices];
		}


		//Quad & Circle Indicies
		{
			u32* quadAndCircleIndices = new u32[s_Data.k_MaxIndices];

			u32 offset = 0;
			for (u32 i = 0; i < s_Data.k_MaxIndices; i += 6)
			{
				quadAndCircleIndices[i + 0] = offset + 0;
				quadAndCircleIndices[i + 1] = offset + 1;
				quadAndCircleIndices[i + 2] = offset + 2;

				quadAndCircleIndices[i + 3] = offset + 2;
				quadAndCircleIndices[i + 4] = offset + 3;
				quadAndCircleIndices[i + 5] = offset + 0;

				offset += 4;
			}

			//Quad Indicies
			{
				std::shared_ptr<IndexBuffer> quadIB;
				quadIB.reset(IndexBuffer::Create(quadAndCircleIndices, s_Data.k_MaxIndices));
				s_Data.m_QuadVertexArray->SetIndexBuffer(quadIB);
			}
			//Circle Indicies
			{
				std::shared_ptr<IndexBuffer> circleIB;
				circleIB.reset(IndexBuffer::Create(quadAndCircleIndices, s_Data.k_MaxIndices));
				s_Data.m_CircleVertexArray->SetIndexBuffer(circleIB);
			}

			delete[] quadAndCircleIndices;
		}

		//Textures
		{
			s_Data.m_WhiteTexture.reset(Texture::Create(1u, 1u));
			const u32 whiteTextureData = 0xffffffff;
			s_Data.m_WhiteTexture->SetData(&whiteTextureData, sizeof(u32));
			s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;
		}
	}
	/*static*/ void BatchedRenderer2D::Shutdown()
	{
		delete[] s_Data.m_QuadVertexBufferBase;
	}

	/*static*/ void BatchedRenderer2D::BeginScene()
	{
		s_Data.m_Stats.m_QuadCount = 0;
		s_Data.m_Stats.m_CircleCount = 0;
		s_Data.m_Stats.m_DrawCalls = 0;

		s_Data.m_QuadShader->Bind();
		s_Data.m_QuadShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());

		s_Data.m_CircleShader->Bind();
		s_Data.m_CircleShader->SetUniform("u_CameraViewProjection", Renderer::GetCamera().GetViewProjectionMatrix());

		StartBatch();
	}
	/*static*/ void BatchedRenderer2D::EndScene()
	{
		FlushBatch();

		//LOG("Quads: {}, Circles: {}, DrawCalls: {}", s_Data.m_Stats.m_QuadCount, s_Data.m_Stats.m_CircleCount, s_Data.m_Stats.m_DrawCalls);
	}

	/*static*/ void BatchedRenderer2D::StartBatch()
	{
		s_Data.m_QuadIndexCount = 0;
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;

		s_Data.m_CircleIndexCount = 0;
		s_Data.m_CircleVertexBufferPtr = s_Data.m_CircleVertexBufferBase;
	}
	/*static*/ void BatchedRenderer2D::FlushBatch()
	{
		//TODO: There is an inefficiency that can't flush independantly of each other

		//Flush Quads
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

		//Flush Circles
		if (s_Data.m_CircleIndexCount)
		{
			const u32 dataSize = (u32)((u8*)s_Data.m_CircleVertexBufferPtr - (u8*)s_Data.m_CircleVertexBufferBase);
			s_Data.m_CircleVertexBuffer->SetData(s_Data.m_CircleVertexBufferBase, dataSize);

			s_Data.m_CircleShader->Bind();

			//TODO: Look into passing vertex array to DrawIndexed() and binding within
			s_Data.m_CircleVertexArray->Bind();
			GetRendererAPI()->DrawIndexed(s_Data.m_CircleIndexCount);

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
		SCOPED_PROFILER();

		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		if (s_Data.m_QuadIndexCount >= RenderData2D::k_MaxIndices)
			NextBatch();

		for (size_t i = 0; i < k_FullTextureCoords.size(); i++)
		{
			s_Data.m_QuadVertexBufferPtr->Position = transform.MakeTransformMatrix() * s_Data.m_QuadAndCircleVertexPositions[i];
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
		SCOPED_PROFILER();

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
			s_Data.m_QuadVertexBufferPtr->Position = transform.MakeTransformMatrix() * s_Data.m_QuadAndCircleVertexPositions[i];
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

	/*static*/ void BatchedRenderer2D::DrawCircle(const ftransform2D& transform, const fcolor4& color)
	{
		SCOPED_PROFILER();

		if (s_Data.m_CircleIndexCount >= RenderData2D::k_MaxIndices)
			NextBatch();

		for (size_t i = 0; i < k_FullTextureCoords.size(); i++)
		{
			s_Data.m_CircleVertexBufferPtr->Position = transform.MakeTransformMatrix() * s_Data.m_QuadAndCircleVertexPositions[i];
			s_Data.m_CircleVertexBufferPtr->Color = color;
			//s_Data.m_CircleVertexBufferPtr->EntityID = entityID;
			s_Data.m_CircleVertexBufferPtr++;
		}

		s_Data.m_CircleIndexCount += 6;

		s_Data.m_Stats.m_CircleCount++;
	}
}