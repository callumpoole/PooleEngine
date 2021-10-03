
#include "renderer.h"
#include "shader_loader.h"
#include "shader_literals/shader_literals.h"

namespace Poole::Rendering
{
	GLuint Renderer::m_shaderProgramID;
	GLuint Renderer::m_shaderProgramIDuniformColor;
	std::vector<std::unique_ptr<IVirtualMesh>> Renderer::m_meshes;
	GLuint Renderer::m_vertexbuffer;

	void Renderer::Init()
	{
		// Create and compile our GLSL program from the shaders
		m_shaderProgramID = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::VertexColor::vertexShader,
			Rendering::ShaderLiterals::VertexColor::fragmentShader
		);

		// Create and compile our GLSL program from the shaders
		m_shaderProgramIDuniformColor = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::UniformColor::vertexShader,
			Rendering::ShaderLiterals::UniformColor::fragmentShader
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
		for (std::unique_ptr<IVirtualMesh>& ptr : m_meshes)
		{
			ptr->Render(ptr->UsesUniformColor() ? m_shaderProgramIDuniformColor : m_shaderProgramID);
		}
	}
}