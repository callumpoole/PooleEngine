#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;
	class SubImage;
	class Texture; //Warning, this is a forward decl for something that isn't publicly accessible

	class Renderer2D
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



		//static void DrawTriangle(fvec3 p1, fvec3 p2, fvec3 p3, fcolor4 color);
		//static void DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor4 color, f32 zLayer = 0.f);
		//static void DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor3 color, f32 zLayer = 0.f);
		//static void DrawTriangle(Vertex3Color4 p1, Vertex3Color4 p2, Vertex3Color4 p3);
		//static void DrawTriangle(Vertex3Color3 p1, Vertex3Color3 p2, Vertex3Color3 p3);
		//static void DrawTriangle(Vertex2Color4 p1, Vertex2Color4 p2, Vertex2Color4 p3, f32 zLayer = 0.f);
		//static void DrawTriangle(Vertex2Color3 p1, Vertex2Color3 p2, Vertex2Color3 p3, f32 zLayer = 0.f);
	};
}
