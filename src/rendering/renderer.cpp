
#include "renderer.h"
#include "shader_loader.h"
#include "shader_literals/shader_literals.h"

namespace Poole::Rendering
{
	GLuint Renderer::m_programID;
	std::vector<std::unique_ptr<IMesh>> Renderer::m_meshes;
	GLuint Renderer::m_vertexbuffer;

	void Renderer::Init()
	{
		// Create and compile our GLSL program from the shaders
		m_programID = Rendering::LoadShaderLiterals(
			Rendering::ShaderLiterals::currentVertexShader,
			Rendering::ShaderLiterals::currentFragmentShader
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

		//Use Shader
		glUseProgram(m_programID);

		RenderAll();

		//Swap front and back buffers
		glfwSwapBuffers(window);
	}

	void Renderer::RenderAll()
	{
		for (std::unique_ptr<IMesh>& ptr : m_meshes)
		{
			ptr->Render();
		}
	}
}