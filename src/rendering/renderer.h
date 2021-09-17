#pragma once

#include <vector>
#include <memory>
#include "core.h"

//TO MOVE: to poole_primitives.h
namespace Poole 
{
	using Color = glm::vec3;
	using Vertex = glm::vec3;
	struct VertexWithColor
	{
		Vertex position;
		Color m_color;
	};

	template<typename TVertex>
	struct MeshBasic
	{
		std::vector<TVertex> m_verts;
		std::vector<int> m_indicies;
		GLuint m_vertexbuffer; //#todo: Giving each its own buffer might fragment memory
	};
	struct MeshBasicSolidColor
	{
		MeshBasic<Vertex> m_mesh;
		Color m_color;
	};
	using MeshBasicVertexColor = MeshBasic<VertexWithColor>;
}
// END MOVE

struct GLFWwindow;

namespace Poole::Rendering {

	class Renderer
	{
	public:
		static MeshBasicSolidColor* Submit(const MeshBasicSolidColor& meshAndColor);
		static MeshBasicSolidColor* Submit(MeshBasicSolidColor&& meshAndColor);
		//MeshBasicVertexColor* Submit(const MeshBasicVertexColor& m_mesh);
		//MeshBasicVertexColor* Submit(MeshBasicVertexColor&& m_mesh);

		static void Init();
		static void Tick(GLFWwindow* window);
	private:
		static void RenderAll();
		static MeshBasicSolidColor* InitLastSumbitted();

		static GLuint m_programID;
		static std::vector<std::unique_ptr<MeshBasicSolidColor>> m_solidColorMeshes;
		static std::vector<std::unique_ptr<MeshBasicVertexColor>> m_vertexColorMeshes;
		static GLuint m_vertexbuffer;

		static void RenderObject(MeshBasicSolidColor* meshAndColor);
		//void RenderObject(MeshBasicVertexColor* m_mesh);
	};
}