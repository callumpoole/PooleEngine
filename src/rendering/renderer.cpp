
#include "poole/rendering/renderer.h"

#include <iostream>

#include "rendering/renderer2D.h"
#include "rendering/batched_renderer2D.h"
#include "rendering/graphics_api/renderer_api.h"
#include "rendering/image/image.h"

namespace Poole::Rendering
{
	EGraphicsAPI Renderer::s_GraphicsAPI = EGraphicsAPI::OpenGL;

	OrthographicCamera Renderer::s_camera;
	GLShader Renderer::s_shaderUniformColor;
	GLShader Renderer::s_shaderVertexColor;
	GLShader Renderer::s_shaderUniformColorTransform2D;
	GLShader Renderer::s_shaderVertexColorTransform2D;
	GLShader Renderer::s_shaderCircleTransform2D;
	GLShader Renderer::s_shaderTextureTransform2D;
	GLShader Renderer::s_shaderTextureBatchedTransform2D;
	GLShader Renderer::s_shaderExperimental1;
	GLShader Renderer::s_shaderExperimental2;
	GLShader Renderer::s_shaderExperimental3;
	std::vector<std::unique_ptr<IMeshBase>> Renderer::s_meshes;

	void Renderer::Init()
	{
		//TODO: Move to OpenGL_RendererAPI:
		Image::SetYFlipBeforeLoad(s_GraphicsAPI == EGraphicsAPI::OpenGL);

		s_camera.EnableResizeWithWindow();
		s_camera.SetZoomScale(1.f);
		//m_camera.GetBounds();
		LoadShaders();

		Renderer2D::Init();
		BatchedRenderer2D::Init();
	}
	void Renderer::BeginScene()
	{
		//Draw BG
		GetRendererAPI()->SetClearColor({ 0.2f, 0.3f, 0.3f, 1.0f });
		GetRendererAPI()->Clear();

		RenderMeshesOldWay();

		Renderer2D::BeginScene();
		BatchedRenderer2D::BeginScene();
	}
	void Renderer::RenderScene()
	{
		Renderer2D::RenderScene();
	}
	void Renderer::EndScene(GLFWwindow* window)
	{
		Renderer2D::EndScene();
		BatchedRenderer2D::EndScene();

		//Swap front and back buffers
		glfwSwapBuffers(window);
	}

	void Renderer::RenderMeshesOldWay()
	{
		for (std::unique_ptr<IMeshBase>& ptr : s_meshes)
		{
			Shader* shader;
			if (ptr->Uses2DTransform())
			{
				shader = ptr->UsesUniformColor4() ? &s_shaderUniformColorTransform2D
												  : &s_shaderVertexColorTransform2D;
			}
			else
			{
				shader = ptr->UsesUniformColor4() ? &s_shaderUniformColor
												  : &s_shaderVertexColor;
			}

			ptr->Render(*shader);
		}
	}

	IMeshBase* Renderer::GetMesh(i32 index) 
	{ 
		if (index >= 0 && index < s_meshes.size())
		{
			return s_meshes[index].get();
		}
		else
		{
			LOG_ERROR("tried to access mesh at index: {} but size is {}", index, s_meshes.size());
			return nullptr;
		}
	}

	void Renderer::LoadShaders()
	{
		//TODO: Fix up working dir / solution dir perhaps. Maybe compile shaders from solution into working DEBUG/
		// Create and compile our GLSL program from the shaders

		#define shader_path "../../poole_engine/src/rendering/shaders/"
		s_shaderUniformColor = Rendering::GLShader(shader_path "UniformColor.shader");
		s_shaderVertexColor = Rendering::GLShader(shader_path "VertexColor.shader");
		s_shaderUniformColorTransform2D = Rendering::GLShader(shader_path "UniformColor2DTransform.shader");
		s_shaderVertexColorTransform2D = Rendering::GLShader(shader_path "VertexColor2DTransform.shader");
		s_shaderCircleTransform2D = Rendering::GLShader(shader_path "Circle2DTransform.shader");
		s_shaderTextureTransform2D = Rendering::GLShader(shader_path "Texture2DTransform.shader");
		s_shaderTextureBatchedTransform2D = Rendering::GLShader(shader_path "Texture2DBatchedTransform.shader");
		//s_shaderExperimental1 = Rendering::GLShader(shader_path "Experimental1.shader");
		//s_shaderExperimental2 = Rendering::GLShader(shader_path "Experimental2.shader");
		//s_shaderExperimental3 = Rendering::GLShader(shader_path "Experimental3.shader");
		#undef shader_path
	}
}