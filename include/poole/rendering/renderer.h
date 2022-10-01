#pragma once

#include "poole/core.h"
#include "shader_loader.h"
#include "camera/orthographic_camera.h"

struct GLFWwindow;

namespace Poole::Rendering {

	enum class EGraphicsAPI
	{
		None,
		OpenGL,
	};

	class Renderer
	{
	public:
		static EGraphicsAPI s_GraphicsAPI;

		template<typename MeshType>
		static MeshType* Submit(const MeshType& meshAndColor);
		template<typename MeshType>
		static MeshType* Submit(MeshType&& meshAndColor);

		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene(GLFWwindow* window);

		static OrthographicCamera& GetCamera() { return s_camera; }

		static GLShader s_shaderUniformColor;
		static GLShader s_shaderVertexColor;
		static GLShader s_shaderUniformColorTransform2D;
		static GLShader s_shaderVertexColorTransform2D; 
		static GLShader s_shaderCircleTransform2D;
		static GLShader s_shaderCircleBatchedTransform2D;
		static GLShader s_shaderLineBatchedTransform2D;
		static GLShader s_shaderTextureTransform2D;
		static GLShader s_shaderTextureBatchedTransform2D;
		static GLShader s_shaderExperimental1;
		static GLShader s_shaderExperimental2;
		static GLShader s_shaderExperimental3;

	private:
		static void LoadShaders();

		static OrthographicCamera s_camera;
	};
}