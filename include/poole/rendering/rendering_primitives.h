#pragma once

#include "poole/core.h"
#include <type_traits>
#include "shader_loader.h"
#include "camera/orthographic_camera.h"
#include "renderer2D.h"

namespace Poole::Rendering
{
	struct IMeshBase
	{
		virtual void Init() {}
		virtual void Render(Shader& /*shader*/) {}
		//#todo: find a way to make this constexpr / remove it
		virtual bool UsesUniformColor4() const { return false; }
		virtual bool Uses2DTransform() const { return false; }
		virtual void SetUniforms(Shader& shader);
	};

	template<typename ... TDecorators>
	struct IMesh
		: public IMeshBase
		, public TDecorators...
	{
		static_assert((std::is_base_of<IMeshDecoratorBase, TDecorators>::value && ...),
			"All TDecorators must inherit from IMeshUniformDataBase struct.");

		template<typename T>
		constexpr bool ContainsDecorator() const
		{
			return (std::is_base_of<T, TDecorators>::value || ...);
		}

		virtual bool UsesUniformColor4() const override
		{
			return ContainsDecorator<MeshUniform_SolidColor<fvec4>>();
		}
		virtual bool Uses2DTransform() const override
		{
			return ContainsDecorator<MeshUniform_DynamicTransform<fmat3>>();
		}
		virtual void SetUniforms(Shader& shader) override
		{
			IMeshBase::SetUniforms(shader); //Super
			(TDecorators::SetInternalUniforms(shader), ...);
		}
	};



	//template <typename T>
	//concept IMesh2 = requires(T& t, const T& tconst) {
	//	t.InternalInit();
	//	t.Init();
	//	t.Render((GLuint)0);
	//	{ tconst.UsesUniformColor3() } -> std::same_as<bool>;
	//	{ tconst.Uses2DTransform() } -> std::same_as<bool>;
	//	t.SetUniforms((GLuint)0);
	//};


	struct IMeshDecoratorBase
	{
		virtual void SetInternalUniforms(Shader& /*shader*/) { }
	};

	template<typename T>
	struct MeshUniform_SolidColor : public IMeshDecoratorBase
	{
		void SetInternalUniforms(Shader& shader)
		{
			IMeshDecoratorBase::SetInternalUniforms(shader); /*Super*/
			shader.SetUniform("u_Color", m_color);
		}
		T m_color{};
	};


	template<typename T>
	struct MeshUniform_DynamicTransform { };

	template<>
	struct MeshUniform_DynamicTransform<fmat3> : public IMeshDecoratorBase
	{																	
		void SetInternalUniforms(Shader& shader)
		{
			IMeshDecoratorBase::SetInternalUniforms(shader); /*Super*/
			shader.SetUniform("u_Transform", Renderer2D::MakeTransformMatrix(m_position, m_scale, m_rotation, m_shear));
		}
		fvec2 m_position{ 0.f, 0.f };
		fvec2 m_scale{1.f, 1.f};
		f32 m_rotation = 0;
		fvec2 m_shear{0.f, 0.f};
	};






	struct MeshFeature_HasVertsBase {};
	template<typename _TVertex>
	struct MeshFeature_HasVerts : public MeshFeature_HasVertsBase, public IMeshDecoratorBase
	{
		using TVertex = _TVertex;
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer = 0; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasIndicies : public IMeshDecoratorBase
	{
		std::vector<int> m_indices;
		GLuint m_elementbuffer = 0; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasNormals : public IMeshDecoratorBase {}; //TODO
	struct MeshFeature_HasUVs : public IMeshDecoratorBase {}; //TODO











	struct StaticMeshNoIndiciesSolidColor4_2DTransform
		: public IMesh<MeshFeature_HasVerts<fvec3>,
					   MeshUniform_SolidColor<fvec4>, 
					   MeshUniform_DynamicTransform<fmat3>>
	{
		virtual void Init() override;
		virtual void Render(Shader& shader) override;
	};

	struct StaticMeshSolidColor4_2DTransform
		: public IMesh<MeshFeature_HasVerts<fvec3>,
					   MeshFeature_HasIndicies,
					   MeshUniform_SolidColor<fvec4>,
					   MeshUniform_DynamicTransform<fmat3>>
	{
		virtual void Init() override;
		virtual void Render(Shader& shader) override;
	};


	struct StaticMeshVertexColor4_2DTransform
		: public IMesh<MeshFeature_HasVerts<Vertex3Color4>,
					   MeshFeature_HasIndicies,
					   MeshUniform_DynamicTransform<fmat3>>
	{
		virtual void Init() override;
		virtual void Render(Shader& shader) override;
	};
}