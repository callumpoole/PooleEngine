#pragma once

#include <vector>
#include <memory>
#include "core.h"
#include "rendering_primitives.h"
#include "shader_loader.h"

struct GLFWwindow;

namespace Poole::Rendering {

	class Renderer
	{
	public:
		template<typename MeshType>
		static MeshType* Submit(const MeshType& meshAndColor);
		template<typename MeshType>
		static MeshType* Submit(MeshType&& meshAndColor);

		static void Init();
		static void Tick(GLFWwindow* window);

		static IMeshBase* GetMesh(i32 index) { return m_meshes[index].get(); } //TODO: BoundsCheck
		template<typename T>
		static T* GetMesh(i32 index) { return dynamic_cast<T*>(m_meshes[index].get()); } //TODO: BoundsCheck

	private:
		static void LoadShaders();
		static void RenderAll();

		static GLShader m_shaderUniformColor;
		static GLShader m_shaderVertexColor;
		static GLShader m_shaderUniformColorTransform2D;
		static GLShader m_shaderVertexColorTransform2D;
		static GLShader m_shaderExperimental1;
		static GLShader m_shaderExperimental2;
		static GLShader m_shaderExperimental3;
		static std::vector<std::unique_ptr<IMeshBase>> m_meshes;
		static GLuint m_vertexbuffer;
	};

	//--

	template<typename MeshType>
	/*static*/ MeshType* Renderer::Submit(const MeshType& meshAndColor)
	{
		m_meshes.emplace_back(new MeshType(meshAndColor));
		m_meshes.back()->Init();
		return dynamic_cast<MeshType*>(m_meshes.back().get());
	}
	template<typename MeshType>
	/*static*/ MeshType* Renderer::Submit(MeshType&& meshAndColor)
	{
		m_meshes.emplace_back(new MeshType(std::move(meshAndColor)));
		m_meshes.back()->Init();
		return dynamic_cast<MeshType*>(m_meshes.back().get());
	}
}