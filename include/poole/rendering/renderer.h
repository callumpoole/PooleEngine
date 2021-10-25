#pragma once

#include "poole/core.h"
#include "rendering_primitives.h"
#include "shader_loader.h"
#include "camera/orthographic_camera.h"

struct GLFWwindow;

namespace Poole::Rendering {

	enum class GraphicsAPI
	{
		None,
		OpenGL,
	};

	class Renderer
	{
	public:
		static GraphicsAPI s_GraphicsAPI;

		template<typename MeshType>
		static MeshType* Submit(const MeshType& meshAndColor);
		template<typename MeshType>
		static MeshType* Submit(MeshType&& meshAndColor);

		static void Init();
		static void Tick(GLFWwindow* window);

		static IMeshBase* GetMesh(i32 index);
		template<typename T>
		static T* GetMesh(i32 index) { return dynamic_cast<T*>(GetMesh(index)); }

		static OrthographicCamera& GetCamera() { return s_camera; }
	private:
		static void LoadShaders();
		static void RenderAll();

		static OrthographicCamera s_camera;
		static GLShader s_shaderUniformColor;
		static GLShader s_shaderVertexColor;
		static GLShader s_shaderUniformColorTransform2D;
		static GLShader s_shaderVertexColorTransform2D;
		static GLShader s_shaderExperimental1;
		static GLShader s_shaderExperimental2;
		static GLShader s_shaderExperimental3;
		static std::vector<std::unique_ptr<IMeshBase>> s_meshes;
		static GLuint s_vertexBuffer;
	};

	//--

	template<typename MeshType>
	/*static*/ MeshType* Renderer::Submit(const MeshType& meshAndColor)
	{
		s_meshes.emplace_back(new MeshType(meshAndColor));
		s_meshes.back()->Init();
		return dynamic_cast<MeshType*>(s_meshes.back().get());
	}
	template<typename MeshType>
	/*static*/ MeshType* Renderer::Submit(MeshType&& meshAndColor)
	{
		s_meshes.emplace_back(new MeshType(std::move(meshAndColor)));
		s_meshes.back()->Init();
		return dynamic_cast<MeshType*>(s_meshes.back().get());
	}
}