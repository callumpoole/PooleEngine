#pragma once

#include <vector>
#include <memory>
#include "core.h"
#include "rendering_primitives.h"

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
	private:
		static void RenderAll();

		static GLuint m_shaderProgramID;
		static GLuint m_shaderProgramIDuniformColor;
		static std::vector<std::unique_ptr<IMesh>> m_meshes;
		static GLuint m_vertexbuffer;
	};

	//--

	template<typename MeshType>
	/*static*/ MeshType* Renderer::Submit(const MeshType& meshAndColor)
	{
		m_meshes.emplace_back(new MeshType(meshAndColor));
		m_meshes.back()->InternalInit();
		return dynamic_cast<MeshType*>(m_meshes.back().get());
	}
	template<typename MeshType>
	/*static*/ MeshType* Renderer::Submit(MeshType&& meshAndColor)
	{
		m_meshes.emplace_back(new MeshType(std::move(meshAndColor)));
		m_meshes.back()->InternalInit();
		return dynamic_cast<MeshType*>(m_meshes.back().get());
	}
}