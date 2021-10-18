#pragma once

#include "poole/core.h"
#include <type_traits>
#include "glm/gtc/type_ptr.hpp"
#include "camera/orthographic_camera.h"

namespace Poole::Rendering
{
	using Vertex = fvec3;

	template<typename _TVertexPart, typename _TColorPart>
	struct VertexWithColor { };

	template<>
	struct VertexWithColor<Vertex, fcolor3>
	{
		using TVertexPart = Vertex;
		using TColorPart = fcolor3;
		union {
			struct
			{
				TVertexPart m_position;
				TColorPart m_color;
			};
			struct { f32 x, y, z, r, g, b; }; //I feel no need for m_ here
		};
	};
	using VertexWithColor3 = VertexWithColor<Vertex, fcolor3>;

	template<>
	struct VertexWithColor<Vertex, fcolor4>
	{
		using TVertexPart = Vertex;
		using TColorPart = fcolor4;
		union {
			struct
			{
				TVertexPart m_position;
				TColorPart m_color;
			};
			struct { f32 x, y, z, r, g, b, a; }; //I feel no need for m_ here
		};
	};
	using VertexWithColor4 = VertexWithColor<Vertex, fcolor4>;



	static void SetUniform(const GLuint programId, const char* uniformName, const f32 f) { glUniform1f(glGetUniformLocation(programId, uniformName), f); }
	static void SetUniform(const GLuint programId, const char* uniformName, const fvec2 v) { glUniform2f(glGetUniformLocation(programId, uniformName), v.x, v.y); }
	static void SetUniform(const GLuint programId, const char* uniformName, const fvec3 v) { glUniform3f(glGetUniformLocation(programId, uniformName), v.x, v.y, v.z); }
	static void SetUniform(const GLuint programId, const char* uniformName, const fvec4 v) { glUniform4f(glGetUniformLocation(programId, uniformName), v.x, v.y, v.z, v.w); }
	static void SetUniform(const GLuint programId, const char* uniformName, const fmat2 v) { glUniformMatrix2fv(glGetUniformLocation(programId, uniformName), 1, GL_TRUE, glm::value_ptr(v)); }
	static void SetUniform(const GLuint programId, const char* uniformName, const fmat3 v) { glUniformMatrix3fv(glGetUniformLocation(programId, uniformName), 1, GL_TRUE, glm::value_ptr(v)); }
	static void SetUniform(const GLuint programId, const char* uniformName, const fmat4 v) { glUniformMatrix4fv(glGetUniformLocation(programId, uniformName), 1, GL_TRUE, glm::value_ptr(v)); }



	struct IMeshBase
	{
		virtual void Init() {}
		virtual void Render(GLuint /*programId*/) {}
		//#todo: find a way to make this constexpr / remove it
		virtual bool UsesUniformColor3() const { return false; }
		virtual bool Uses2DTransform() const { return false; }
		virtual void SetUniforms(GLuint programId);
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

		virtual bool UsesUniformColor3() const override
		{
			return ContainsDecorator<MeshUniform_SolidColor<fvec3>>();
		}
		virtual bool Uses2DTransform() const override
		{
			return ContainsDecorator<MeshUniform_DynamicTransform<fmat3>>();
		}
		virtual void SetUniforms(GLuint programId) override
		{
			IMeshBase::SetUniforms(programId); //Super
			(TDecorators::SetInternalUniforms(programId), ...);
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
		virtual void SetInternalUniforms(const GLuint /*programId*/) { }
	};

	template<typename T>
	struct MeshUniform_SolidColor : public IMeshDecoratorBase
	{
		void SetInternalUniforms(const GLuint programId)
		{
			IMeshDecoratorBase::SetInternalUniforms(programId); /*Super*/
			SetUniform(programId, "u_Color", m_color);
		}
		T m_color{};
	};


	template<typename T>
	struct MeshUniform_DynamicTransform { };

	template<>
	struct MeshUniform_DynamicTransform<fmat3> : public IMeshDecoratorBase
	{																	
		void SetInternalUniforms(const GLuint programId)
		{
			IMeshDecoratorBase::SetInternalUniforms(programId); /*Super*/

			const fmat3 Translation = {
				1, 0, m_position.x,
				0, 1, m_position.y,
				0, 0, 1
			};
			const fmat3 Rotation = {
				cosf(m_rotation), -sinf(m_rotation), 0,
				sinf(m_rotation),  cosf(m_rotation), 0,
							   0,				  0, 1,
			};
			const fmat3 Scale = {
				m_scale.x,		   0, 0,
						0, m_scale.y, 0,
						0,		   0, 1
			};
			const fmat3 Shear = {
				1,		   m_shear.x, 0,
				m_shear.y,		   1, 0,
				0,				   0, 1
			};

			m_transform = Translation * Rotation * Shear * Scale;

			SetUniform(programId, "u_Transform", m_transform);
		}
		fmat3 m_transform;
		fvec2 m_position{ 0.f, 0.f };
		f32 m_rotation = 0;
		fvec2 m_scale{1.f, 1.f};
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











	struct StaticMeshNoIndiciesSolidColor3_2DTransform
		: public IMesh<MeshFeature_HasVerts<Vertex>, 
					   MeshUniform_SolidColor<fvec3>, 
					   MeshUniform_DynamicTransform<fmat3>>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};

	struct StaticMeshSolidColor3_2DTransform
		: public IMesh<MeshFeature_HasVerts<Vertex>,
					   MeshFeature_HasIndicies,
					   MeshUniform_SolidColor<fvec3>,
					   MeshUniform_DynamicTransform<fmat3>>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};


	struct StaticMeshVertexColor3_2DTransform
		: public IMesh<MeshFeature_HasVerts<VertexWithColor3>,
					   MeshFeature_HasIndicies,
					   MeshUniform_DynamicTransform<fmat3>>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};
}