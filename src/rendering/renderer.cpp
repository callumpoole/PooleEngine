
#include "renderer.h"
#include "shader_loader.h"
#include "shader_literals/shader_literals.h"

namespace Poole::Rendering
{
	GLuint Renderer::m_shaderProgramId;
	GLuint Renderer::m_shaderProgramIdUniformColor;
	GLuint Renderer::m_shaderProgramIdTransform2D;
	GLuint Renderer::m_shaderProgramIdUniformColorTransform2D;
	std::vector<std::unique_ptr<IMesh>> Renderer::m_meshes;
	GLuint Renderer::m_vertexbuffer;

	void Renderer::Init()
	{
		// Create and compile our GLSL program from the shaders
		m_shaderProgramId = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::VertexColor::vertexShader,
			Rendering::ShaderLiterals::VertexColor::fragmentShader
		);
		m_shaderProgramIdUniformColor = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::UniformColor::vertexShader,
			Rendering::ShaderLiterals::UniformColor::fragmentShader
		);
		m_shaderProgramIdTransform2D = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::VertexColor_Transform2D::vertexShader,
			Rendering::ShaderLiterals::VertexColor_Transform2D::fragmentShader
		);
		m_shaderProgramIdUniformColorTransform2D = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::UniformColor_Transform2D::vertexShader,
			Rendering::ShaderLiterals::UniformColor_Transform2D::fragmentShader
		);

		//m_programID = Rendering::LoadShaders(
		//    "D:/Callum/Desktop/PooleEngineSandbox/build/Sandbox/Debug/basic_vertex.vertexshader", 
		//    "D:/Callum/Desktop/PooleEngineSandbox/build/Sandbox/Debug/basic_fragment.fragmentshader"
		//    "basic_vertex.vertexshader",
		//    "basic_fragment.fragmentshader"
		//);
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

	void Renderer::RenderAll()
	{
		for (std::unique_ptr<IMesh>& ptr : m_meshes)
		{
			GLuint programId;
			if (ptr->Uses2DTransform())
			{
				programId = ptr->UsesUniformColor() ? m_shaderProgramIdUniformColorTransform2D : m_shaderProgramIdTransform2D;
			}
			else
			{
				programId = ptr->UsesUniformColor() ? m_shaderProgramIdUniformColor : m_shaderProgramId;
			}

			ptr->Render(programId);
		}
	}
}