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

		static void DrawQuad(const ftransform2D& transform, const fcolor4& color);
		static void DrawQuad(const fvec2& pos, const fvec2& scale, const fcolor4& color, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawQuad({ pos, scale, rotation, shear }, color);
		}

		static void DrawSubTexturedQuad(const ftransform2D& transform, const std::shared_ptr<Texture>& texture, const std::array<fvec2, 4> textureCoords, float tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>);
		static void DrawSubTexturedQuad(const ftransform2D& transform, const SubImage& subImage, float tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>);

		static void DrawTexturedQuad(const ftransform2D& transform, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawSubTexturedQuad(transform, texture, k_FullTextureCoords, tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const ftransform2D& transform, const Image& image, float tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>)
		{
			DrawTexturedQuad(transform, GetOrLoadTexture(image), tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const fvec2& pos, const fvec2& scale, const std::shared_ptr<Texture>& texture, float tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawTexturedQuad({ pos, scale, rotation, shear }, texture, tilingFactor, tintColor);
		}
		static void DrawTexturedQuad(const fvec2& pos, const fvec2& scale, const Image& image, float tilingFactor = 1.f, const fcolor4& tintColor = Colors::White<fcolor4>, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawTexturedQuad({ pos, scale, rotation, shear }, GetOrLoadTexture(image), tilingFactor, tintColor);
		}

		static std::shared_ptr<Texture> GetOrLoadTexture(const Image& image);

		static constexpr std::array<fvec2, 4> k_FullTextureCoords = { fvec2{ 0.0f, 0.0f }, fvec2{ 1.0f, 0.0f }, fvec2{ 1.0f, 1.0f }, fvec2{ 0.0f, 1.0f } };
	};
}