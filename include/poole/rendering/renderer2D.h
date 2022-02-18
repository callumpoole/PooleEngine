#pragma once

#include "poole/core.h"

namespace Poole::Rendering
{
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

		//static void DrawTriangle(fvec3 p1, fvec3 p2, fvec3 p3, fcolor4 color);
		//static void DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor4 color, f32 zLayer = 0.f);
		//static void DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor3 color, f32 zLayer = 0.f);
		//static void DrawTriangle(Vertex3Color4 p1, Vertex3Color4 p2, Vertex3Color4 p3);
		//static void DrawTriangle(Vertex3Color3 p1, Vertex3Color3 p2, Vertex3Color3 p3);
		//static void DrawTriangle(Vertex2Color4 p1, Vertex2Color4 p2, Vertex2Color4 p3, f32 zLayer = 0.f);
		//static void DrawTriangle(Vertex2Color3 p1, Vertex2Color3 p2, Vertex2Color3 p3, f32 zLayer = 0.f);
	};
}
