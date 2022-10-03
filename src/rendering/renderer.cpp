
#include "poole/rendering/renderer.h"

#include <iostream>

#include "rendering/renderer2D.h"
#include "rendering/graphics_api/renderer_api.h"
#include "rendering/image/image.h"
#include "rendering/text/text_renderer_factory.h"
#include "poole/rendering/text/text_renderer.h"

namespace Poole::Rendering
{
	EGraphicsAPI Renderer::s_GraphicsAPI = EGraphicsAPI::OpenGL;

	OrthographicCamera Renderer::s_camera;
	GLShader Renderer::s_shaderUniformColor;
	GLShader Renderer::s_shaderVertexColor;
	GLShader Renderer::s_shaderUniformColorTransform2D;
	GLShader Renderer::s_shaderVertexColorTransform2D;
	GLShader Renderer::s_shaderCircleTransform2D;
	GLShader Renderer::s_shaderCircleBatchedTransform2D;
	GLShader Renderer::s_shaderLineBatchedTransform2D;
	GLShader Renderer::s_shaderTextureTransform2D;
	GLShader Renderer::s_shaderTextureBatchedTransform2D;
	GLShader Renderer::s_shaderExperimental1;
	GLShader Renderer::s_shaderExperimental2;
	GLShader Renderer::s_shaderExperimental3;

	void Renderer::Init()
	{
		//TODO: Move to OpenGL_RendererAPI:
		Image::SetYFlipBeforeLoad(s_GraphicsAPI == EGraphicsAPI::OpenGL);

		s_camera.EnableResizeWithWindow();
		s_camera.SetZoomScale(1.f);
		//m_camera.GetBounds();
		LoadShaders();

#if UNBATCHED_RENDERER
		UnbatchedRenderer2D::Init();
#endif
#if BATCHED_RENDERER
		BatchedRenderer2D::Init();
#endif

		TextRendererFactory::Init();
	}
	void Renderer::Shutdown()
	{
#if UNBATCHED_RENDERER
		UnbatchedRenderer2D::Shutdown();
#endif
#if BATCHED_RENDERER
		BatchedRenderer2D::Shutdown();
#endif
	}
	void Renderer::BeginScene()
	{
		//Draw BG
		GetRendererAPI()->SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		GetRendererAPI()->Clear();

#if UNBATCHED_RENDERER
		UnbatchedRenderer2D::BeginScene();
#endif
#if BATCHED_RENDERER
		BatchedRenderer2D::BeginScene();
#endif

		Draw2DAxes();
	}
	void Renderer::EndScene(GLFWwindow* window)
	{
		//Render text on top since it's in EndScene
		TextRendererFactory::RenderAllTextRenderers();

#if UNBATCHED_RENDERER
		UnbatchedRenderer2D::EndScene();
#endif
#if BATCHED_RENDERER
		BatchedRenderer2D::EndScene();
#endif

		//Swap front and back buffers
		glfwSwapBuffers(window);
	}

	void Renderer::LoadShaders()
	{
		//TODO: Fix up working dir / solution dir perhaps. Maybe compile shaders from solution into working DEBUG/
		// Create and compile our GLSL program from the shaders

		#define shader_path "../../poole_engine/src/rendering/shaders/"
		s_shaderUniformColor				= Rendering::GLShader(shader_path "UniformColor.shader");
		s_shaderVertexColor					= Rendering::GLShader(shader_path "VertexColor.shader");
		s_shaderUniformColorTransform2D		= Rendering::GLShader(shader_path "UniformColor2DTransform.shader");
		s_shaderVertexColorTransform2D		= Rendering::GLShader(shader_path "VertexColor2DTransform.shader");
		s_shaderCircleTransform2D			= Rendering::GLShader(shader_path "Circle2DTransform.shader");
		s_shaderCircleBatchedTransform2D	= Rendering::GLShader(shader_path "Circle2DBatchedTransform.shader");
		s_shaderLineBatchedTransform2D		= Rendering::GLShader(shader_path "Line2DBatchedTransform.shader");
		s_shaderTextureTransform2D			= Rendering::GLShader(shader_path "Texture2DTransform.shader");
		s_shaderTextureBatchedTransform2D	= Rendering::GLShader(shader_path "Texture2DBatchedTransform.shader");
		//s_shaderExperimental1				= Rendering::GLShader(shader_path "Experimental1.shader");
		//s_shaderExperimental2				= Rendering::GLShader(shader_path "Experimental2.shader");
		//s_shaderExperimental3				= Rendering::GLShader(shader_path "Experimental3.shader");
		#undef shader_path
	}

	/*static*/ void Renderer::Draw2DAxes()
	{
#if BATCHED_RENDERER
		constexpr bool positiveOnly = false;
		constexpr i32 distanceFromOrigin = 3;
		constexpr f32 min = positiveOnly ? 0.0f : -distanceFromOrigin;
		constexpr f32 max = distanceFromOrigin;
		constexpr f32 markersEvery = 0.1f;
		constexpr f32 markersBiggerEvery = 1.0f;
		constexpr f32 markerSize = 0.01f;
		constexpr f32 biggerMarkerSize = 0.04f;
		constexpr bool numberEachBig = true;
		constexpr f32 numberOffset = 0.05f;
		constexpr bool showGrid = true;
		constexpr fcolor4 gridColor = fcolor4{0.5,0.5,0.5,0.1};
		constexpr fcolor4 bigNumberGridColor = fcolor4{ 0.5,0.5,0.5,0.3 };

		BatchedRenderer2D::DrawLine(fvec3{ min, 0.f, 0.f }, fvec3{ max, 0.f, 0.f }, Colors::Red<fcolor4>);
		BatchedRenderer2D::DrawLine(fvec3{ 0.f, min, 0.f }, fvec3{ 0.f, max, 0.f }, Colors::Blue<fcolor4>);

		if constexpr (markersEvery > 0.f)
		{
			struct AxisData
			{
				fvec3 smallMin, smallMax;
				fvec3 bigMin, bigMax;
				fvec3 gridMin, gridMax;
				fvec3 numberOffset;
				fcolor4 col;
			};

			//Pre-computed at compile time to avoid construction in loop runtime.
			constexpr std::array<AxisData, 2> axisData = {
				AxisData{ //X
					fvec3{ /*overwrite*/ 0, -markerSize, 0.f },		    fvec3{ /*overwrite*/ 0, markerSize, 0.f },
					fvec3{ /*overwrite*/ 0, -biggerMarkerSize, 0.f },   fvec3{ /*overwrite*/ 0, biggerMarkerSize, 0.f },
					fvec3{ /*overwrite*/ 0, -distanceFromOrigin, 0.f }, fvec3{ /*overwrite*/ 0, distanceFromOrigin, 0.f },
					fvec3{ /*overwrite*/ 0, numberOffset, 0.f },
					Colors::Red<fcolor4>,
				},
				AxisData{ //Y
					fvec3{ -markerSize,		    /*overwrite*/ 0, 0.f }, fvec3{ markerSize,	       /*overwrite*/ 0, 0.f },
					fvec3{ -biggerMarkerSize,   /*overwrite*/ 0, 0.f }, fvec3{ biggerMarkerSize,   /*overwrite*/ 0, 0.f },
					fvec3{ -distanceFromOrigin, /*overwrite*/ 0, 0.f }, fvec3{ distanceFromOrigin, /*overwrite*/ 0, 0.f },
					fvec3{ numberOffset,	    /*overwrite*/ 0, 0.f },
					Colors::Blue<fcolor4>,
				}
			};

			//For Axis Numbers
			i32 textIndexCounter = 0;
			static bool s_FirstExecution = true;
			static std::array<std::shared_ptr<TextRenderer>, distanceFromOrigin * (positiveOnly ? 2 : 4)> s_AxisNumbers;

			for (u8 axis = 0; axis < 2; axis++)
			{
				const AxisData& data = axisData[axis];

				auto SetInIndex = [axis](fvec3 vec, const f32 val) -> fvec3
				{
					vec[axis] = val;
					return vec;
				};

				for (f32 f = min; f <= max; f += markersEvery)
				{
					if (!Math::IsNearlyZero(f))
					{
						if (markersBiggerEvery > markersEvery && Math::IsMultipleOf(f, markersBiggerEvery))
						{
							if constexpr (showGrid)
							{
								BatchedRenderer2D::DrawLine(SetInIndex(data.gridMin, f), SetInIndex(data.gridMax, f), bigNumberGridColor);
							}

							BatchedRenderer2D::DrawLine(SetInIndex(data.bigMin, f), SetInIndex(data.bigMax, f), data.col);

							if (numberEachBig && s_FirstExecution)
							{
								s_AxisNumbers[textIndexCounter] = TextRendererFactory::MakeRenderText(false);
								s_AxisNumbers[textIndexCounter]->SetPosition(SetInIndex(data.numberOffset, f));
								s_AxisNumbers[textIndexCounter]->SetScale(0.1f);
								s_AxisNumbers[textIndexCounter]->SetText(std::format("{:.0f}", f));
								s_AxisNumbers[textIndexCounter]->SetColor(data.col);

								++textIndexCounter;
							}
						}
						else
						{
							if constexpr (showGrid)
							{
								BatchedRenderer2D::DrawLine(SetInIndex(data.gridMin, f), SetInIndex(data.gridMax, f), gridColor);
							}

							BatchedRenderer2D::DrawLine(SetInIndex(data.smallMin, f), SetInIndex(data.smallMax, f), data.col);
						}
					}
				}
			}

			s_FirstExecution = false;
		}
#endif
	}
}