#pragma once

#include "core.h"

namespace Poole::Rendering
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene();
		static void Render();
		static void EndScene();

		static void DrawQuad(const fvec2& pos, const fvec2& size, const fcolor4& color);

		//static void DrawTriangle(fvec3 p1, fvec3 p2, fvec3 p3, fcolor4 color);
		//static void DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor4 color, f32 zLayer = 0.f);
		//static void DrawTriangle(fvec2 p1, fvec2 p2, fvec2 p3, fcolor3 color, f32 zLayer = 0.f);
		//static void DrawTriangle(Vertex3Color4 p1, Vertex3Color4 p2, Vertex3Color4 p3);
		//static void DrawTriangle(Vertex3Color3 p1, Vertex3Color3 p2, Vertex3Color3 p3);
		//static void DrawTriangle(Vertex2Color4 p1, Vertex2Color4 p2, Vertex2Color4 p3, f32 zLayer = 0.f);
		//static void DrawTriangle(Vertex2Color3 p1, Vertex2Color3 p2, Vertex2Color3 p3, f32 zLayer = 0.f);
	private:
		struct RenderData
		{
			GLuint m_vertexArrayID;
			class GLShader* m_Shader;
			class VertexBuffer* m_VertexBuffer;
			class IndexBuffer* m_IndexBuffer;
		};
		static RenderData s_RenderData;
	};
}
