
#include "poole/rendering/renderer.h"

#include <iostream>

#include "rendering/renderer2D.h"
#include "rendering/graphics_api/renderer_api.h"
#include "rendering/image/image.h"
#include "rendering/text/text_renderer_factory.h"

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
}