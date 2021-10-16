
#include "renderer.h"

namespace Poole::Rendering
{
	GLShader Renderer::m_shaderUniformColor;
	GLShader Renderer::m_shaderVertexColor;
	GLShader Renderer::m_shaderUniformColorTransform2D;
	GLShader Renderer::m_shaderVertexColorTransform2D;
	GLShader Renderer::m_shaderExperimental1;
	GLShader Renderer::m_shaderExperimental2;
	GLShader Renderer::m_shaderExperimental3;
	std::vector<std::unique_ptr<IMeshBase>> Renderer::m_meshes;
	GLuint Renderer::m_vertexbuffer;

	void Renderer::Init()
	{
		LoadShaders();
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

	void Renderer::LoadShaders()
	{
		//TODO: Fix up working dir / solution dir perhaps. Maybe compile shaders from solution into working DEBUG/
		// Create and compile our GLSL program from the shaders
		m_shaderUniformColor = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/UniformColor.shader"
		);
		m_shaderVertexColor = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/VertexColor.shader"
		);
		m_shaderUniformColorTransform2D = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/UniformColor2DTransform.shader"
		);
		m_shaderVertexColorTransform2D = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/VertexColor2DTransform.shader"
		);
		m_shaderExperimental1 = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/Experimental1.shader"
		);
		m_shaderExperimental2 = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/Experimental2.shader"
		);
		m_shaderExperimental3 = Rendering::GLShader(
			"../../poole_engine/src/rendering/shaders/Experimental3.shader"
		);
	}

	void Renderer::RenderAll()
	{
		for (std::unique_ptr<IMeshBase>& ptr : m_meshes)
		{
			GLuint programId;
			if (ptr->Uses2DTransform())
			{
				programId = ptr->UsesUniformColor3() ? m_shaderUniformColorTransform2D.GetProgramID()
													 : m_shaderVertexColorTransform2D.GetProgramID();
			}
			else
			{
				programId = ptr->UsesUniformColor3() ? m_shaderUniformColor.GetProgramID() 
													 : m_shaderVertexColor.GetProgramID();
			}
			ptr->Render(programId);
		}
	}
}