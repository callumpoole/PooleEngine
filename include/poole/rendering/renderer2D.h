#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
	class Image;
	class Texture; //WARNING THIS IS FORWARD DECL IS IN thE PRIVATe MODULE, but used by sandbox. shared pointer cant delete it!!!

	//TO MOVE:
	class SubTexture
	{
	public:
		SubTexture(std::shared_ptr<Texture> texture, fvec2 min, fvec2 max);

		static SubTexture* Create(std::shared_ptr<Texture> texture, fvec2 coords, fvec2 cellSize, fvec2 spriteSize = { 1,1 });

		std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
		const std::array<fvec2, 4>& GetTexCoords() const { return m_TexCoords; }
	private:
		std::shared_ptr<Texture> m_Texture;
		std::array<fvec2, 4> m_TexCoords;
	};










	using TextureHandle = u32;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene();
		static void RenderScene();
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
		 
		static fmat4 MakeTransformMatrix(const ftransform2D& transform);
		static fmat4 MakeTransformMatrix(const fvec2& pos, const fvec2& scale, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			return MakeTransformMatrix({ pos, scale, rotation, shear });
		}

		static TextureHandle LoadTexture(const char* path, bool hasAlpha);
		static TextureHandle LoadTexture(const Image& image, bool hasAlpha);
		static void DrawTexturedQuad(const ftransform2D& transform, TextureHandle handle);
		static void DrawTexturedQuad(const fvec2& pos, const fvec2& scale, TextureHandle handle, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawTexturedQuad({ pos, scale, rotation, shear }, handle);
		}

		static void DrawSubTexturedQuad(const ftransform2D& transform, SubTexture* subTexture);
		static void DrawSubTexturedQuad(const fvec2& pos, const fvec2& scale, SubTexture* subTexture, f32 rotation = 0, const fvec2& shear = fvec2(0.f))
		{
			DrawSubTexturedQuad({ pos, scale, rotation, shear }, subTexture);
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
