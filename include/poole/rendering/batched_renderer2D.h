#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;
	class SubImage;
	class Texture; //Warning, this is a forward decl for something that isn't publicly accessible

	class BatchedRenderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		static void StartBatch();
		static void FlushBatch();
		static void NextBatch();


		static void DrawQuad(const std::array<fvec4, 4>& coords, const ftransform2D& transform, const fcolor4& color);
		static void DrawQuad(const ftransform2D& transform, const fcolor4& color)
		{
			DrawQuad(m_QuadAndCircleVertexPositions, transform, color);
		}
		static void DrawQuad(const fvec2& pos, const fvec2& scale, const fcolor4& color, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawQuad({ pos, scale, rotation, shear }, color);
		}


		static void DrawSubTexturedQuad(const std::array<fvec4, 4>& coords, const ftransform2D& transform, const std::shared_ptr<Texture>& texture, const std::array<fvec2, 4>& textureCoords, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>);
		static void DrawSubTexturedQuad(const ftransform2D& transform, const std::shared_ptr<Texture>& texture, const std::array<fvec2, 4>& textureCoords, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawSubTexturedQuad(m_QuadAndCircleVertexPositions, transform, texture, textureCoords, tilingFactor, tintColor);
		}
		static void DrawSubTexturedQuad(const std::array<fvec4, 4>& coords, const ftransform2D& transform, const std::shared_ptr<Image>& image, const std::array<fvec2, 4>& textureCoords, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawSubTexturedQuad(coords, transform, GetOrLoadTexture(*image), textureCoords, tilingFactor, tintColor);
		}

		static void DrawSubTexturedQuad(const std::array<fvec4, 4>& coords, const ftransform2D& transform, const SubImage& subImage, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>);
		static void DrawSubTexturedQuad(const ftransform2D& transform, const SubImage& subImage, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawSubTexturedQuad(m_QuadAndCircleVertexPositions, transform, subImage, tilingFactor, tintColor);
		}
		static void DrawSubTexturedQuad(const fvec2& pos, const fvec2& scale, const std::shared_ptr<Texture>& texture, const std::array<fvec2, 4>& textureCoords, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawSubTexturedQuad({ pos, scale, rotation, shear }, texture, textureCoords, tilingFactor, tintColor);
		}
		static void DrawSubTexturedQuad(const fvec2& pos, const fvec2& scale, const SubImage& subImage, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawSubTexturedQuad({ pos, scale, rotation, shear }, subImage, tilingFactor, tintColor);
		}


		static void DrawTexturedQuad(const std::array<fvec4, 4>& coords, const ftransform2D& transform, const std::shared_ptr<Texture>& texture, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawSubTexturedQuad(coords, transform, texture, k_FullTextureCoords, tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const ftransform2D& transform, const std::shared_ptr<Texture>& texture, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawSubTexturedQuad(transform, texture, k_FullTextureCoords, tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const ftransform2D& transform, const Image& image, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawTexturedQuad(transform, GetOrLoadTexture(image), tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const fvec2& pos, const fvec2& scale, const std::shared_ptr<Texture>& texture, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawTexturedQuad({ pos, scale, rotation, shear }, texture, tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const fvec2& pos, const fvec2& scale, const Image& image, f32 tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawTexturedQuad({ pos, scale, rotation, shear }, GetOrLoadTexture(image), tilingFactor, tintColor);
		}


		static std::shared_ptr<Texture> GetOrLoadTexture(const Image& image);

		static constexpr std::array<fvec4, 4> m_QuadAndCircleVertexPositions = {
			fvec4{ -0.5f, -0.5f, 0.0f, 1.0f },
			fvec4{ 0.5f, -0.5f, 0.0f, 1.0f },
			fvec4{ 0.5f,  0.5f, 0.0f, 1.0f },
			fvec4{ -0.5f,  0.5f, 0.0f, 1.0f }
		};
		static constexpr std::array<fvec2, 4> k_FullTextureCoords = { fvec2{ 0.0f, 0.0f }, fvec2{ 1.0f, 0.0f }, fvec2{ 1.0f, 1.0f }, fvec2{ 0.0f, 1.0f } };


		static void DrawCircle(const ftransform2D& transform, const fcolor4& color, float thickness = 1.f, float fade = 0.f);
		static void DrawCircle(const fvec2& pos, const fvec2& scale, const fcolor4& color, f32 rotation = 0, const fvec2& shear = fvec2(0.f), float thickness = 1.f, float fade = 0.f)
		{
			DrawCircle({ pos, scale, rotation, shear }, color, thickness, fade);
		}


		static void DrawLine(const fvec3& p0, const fvec3& p1, const fvec4& color);
		static void DrawLine(const fvec2& p0, const fvec2& p1, const fvec4& color)
		{
			DrawLine(fvec3{ p0.x, p0.y, 0.f }, fvec3{ p1.x, p1.y, 0.f }, color);
		}
		static f32 GetLineWidth();
		static void SetLineWidth(f32 width);
	};
}