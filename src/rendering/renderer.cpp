
#include "poole/rendering/renderer.h"

#include <iostream>

#include "rendering/renderer2D.h"

namespace Poole::Rendering
{
	EGraphicsAPI Renderer::s_GraphicsAPI = EGraphicsAPI::OpenGL;

	OrthographicCamera Renderer::s_camera;
	GLShader Renderer::s_shaderUniformColor;
	GLShader Renderer::s_shaderVertexColor;
	GLShader Renderer::s_shaderUniformColorTransform2D;
	GLShader Renderer::s_shaderVertexColorTransform2D;
	GLShader Renderer::s_shaderExperimental1;
	GLShader Renderer::s_shaderExperimental2;
	GLShader Renderer::s_shaderExperimental3;
	std::vector<std::unique_ptr<IMeshBase>> Renderer::s_meshes;
	GLuint Renderer::s_vertexBuffer;

	void Renderer::Init()
	{
		s_camera.UseCameraSizeWithScale(1.f);
		//m_camera.GetBounds();
		LoadShaders();

		Renderer2D::Init();
	}
	void Renderer::Tick(GLFWwindow* window)
	{
		//Draw BG
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderAll();

		//Swap front and back buffers
		glfwSwapBuffers(window);
	}

	IMeshBase* Renderer::GetMesh(i32 index) 
	{ 
		if (index >= 0 && index < s_meshes.size())
		{
			return s_meshes[index].get();
		}
		else
		{
			std::cerr << "tried to access mesh at index: " << index << "but size is " << s_meshes.size() << ".\n";
			return nullptr;
		}
	}

	void Renderer::LoadShaders()
	{
		//TODO: Fix up working dir / solution dir perhaps. Maybe compile shaders from solution into working DEBUG/
		// Create and compile our GLSL program from the shaders
		s_shaderUniformColor = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/UniformColor.shader"
		);
		s_shaderVertexColor = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/VertexColor.shader"
		);
		s_shaderUniformColorTransform2D = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/UniformColor2DTransform.shader"
		);
		s_shaderVertexColorTransform2D = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/VertexColor2DTransform.shader"
		);
		//s_shaderExperimental1 = Rendering::GLShader(
		//	"../../poole_engine/src/rendering/shaders/Experimental1.shader"
		//);
		//s_shaderExperimental2 = Rendering::GLShader(
		//	"../../poole_engine/src/rendering/shaders/Experimental2.shader"
		//);
		//s_shaderExperimental3 = Rendering::GLShader(
		//	"../../poole_engine/src/rendering/shaders/Experimental3.shader"
		//);
	}

	void Renderer::RenderAll()
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

		Renderer2D::BeginScene();
		Renderer2D::Render();
		Renderer2D::EndScene();
	}
}