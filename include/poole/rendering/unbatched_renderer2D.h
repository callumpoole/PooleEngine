#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;
	class SubImage;
	class Texture; //Warning, this is a forward decl for something that isn't publicly accessible

	class UnbatchedRenderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();


		static void DrawQuad(const ftransform2D& transform, const fcolor4& color);
		static void DrawQuad(const fvec2& pos, const fvec2& scale, const fcolor4& color, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawQuad({ pos, scale, rotation, shear }, color);
		}

		static void DrawCircle(const ftransform2D& transform, const fcolor4& color, float thickness = 1.f, float fade = 0.f);
		static void DrawCircle(const fvec2& pos, const fvec2& scale, const fcolor4& color, f32 rotation = 0, const fvec2& shear = fvec2(0.f), float thickness = 1.f, float fade = 0.f)
		{
			DrawCircle({ pos, scale, rotation, shear }, color, thickness, fade);
		}

	private:
		static std::shared_ptr<Texture> GetOrLoadTexture(const Image& image);
	public:
		static void DrawTexturedQuad(const ftransform2D& transform, const Image& image);
		static void DrawTexturedQuad(const fvec2& pos, const fvec2& scale, const Image& image, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawTexturedQuad({ pos, scale, rotation, shear }, image);
		}

		static void DrawSubTexturedQuad(const ftransform2D& transform, const SubImage& subImage);
		static void DrawSubTexturedQuad(const fvec2& pos, const fvec2& scale, const SubImage& subImage, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawSubTexturedQuad({ pos, scale, rotation, shear }, subImage);
		}
	};
}
