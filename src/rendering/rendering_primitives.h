#pragma once

#include "core.h"
#include <vector>
#include <type_traits>

namespace Poole::Rendering
{
	using Vertex = fvec3;

#pragma warning(disable : 4201) //DISABLE nameless struct/union
	struct VertexWithColor3
	{
		union {
			struct
			{
				Vertex m_position;
				fcolor3 m_color;
			};
			struct { f32 x, y, z, r, g, b; }; //I feel no need for m_ here
		};
	};
	struct VertexWithColor4
	{
		union {
			struct
			{
				Vertex m_position;
				fcolor4 m_color;
			};
			struct { f32 x, y, z, r, g, b, a; }; //I feel no need for m_ here
		};
	};
#pragma warning(default : 4201) //DEFAULT nameless struct/union


	struct IMesh
	{
		void InternalInit() //Could remove if nothing else is needed, Super::Would probably work
		{
			Init();
		}
		virtual void Init() {}
		virtual void Render(GLuint programId) {}
		//#todo: find a way to make this constexpr / remove it
		bool UsesUniformColor() const;
		bool Uses2DTransform() const;
		void SetUniforms(GLuint programId);
	};

	//template <typename T>
	//concept IMesh2 = requires(T& t, const T& tconst) {
	//	t.InternalInit();
	//	t.Init();
	//	t.Render((GLuint)0);
	//	{ tconst.UsesUniformColor() } -> std::same_as<bool>;
	//	{ tconst.Uses2DTransform() } -> std::same_as<bool>;
	//	t.SetUniforms((GLuint)0);
	//};




	struct IMeshUniformDataBase
	{
		virtual void SetInternalUniforms(const GLuint programId) { }

		template<typename TFloats> void SetUniform(const GLint uniform, const TFloats f) { assert(0); }
		template<> void SetUniform(const GLint uniform, const f32 f)   { glUniform1f(uniform, f); }
		template<> void SetUniform(const GLint uniform, const fvec2 v) { glUniform2f(uniform, v.x, v.y); }
		template<> void SetUniform(const GLint uniform, const fvec3 v) { glUniform3f(uniform, v.x, v.y, v.z); }
		template<> void SetUniform(const GLint uniform, const fvec4 v) { glUniform4f(uniform, v.x, v.y, v.z, v.w); }
	};

#define MakeUniform(Name, UniformName, MemberName)									\
	struct MeshUniform_##Name##Base {};												\
	template<typename T>															\
	struct MeshUniform_##Name														\
		: public MeshUniform_##Name##Base											\
		, public IMeshUniformDataBase												\
	{																				\
		virtual void SetInternalUniforms(const GLuint programId) override			\
		{																			\
			SetUniform(glGetUniformLocation(programId, UniformName), MemberName);	\
		}																			\
		T MemberName;																\
	};

	MakeUniform(SolidColor,		 "uniformColor",	m_color);
	MakeUniform(DynamicPosition, "uniformPosition", m_position);
	MakeUniform(DynamicRotation, "uniformRotation", m_rotation);
	MakeUniform(DynamicScale,	 "uniformScale",	m_scale);


	#define MeshUniformCollection_2DTransform \
		  MeshUniform_DynamicPosition<fvec2> \
		, MeshUniform_DynamicRotation<f32> \
		, MeshUniform_DynamicScale<fvec2>







	struct IMeshUniformCollectorBase
	{
		virtual void SetAllUniforms(GLuint programId) {}
	};

	template<class ... TDecorators>
	struct IMeshUniformCollector 
		: public IMeshUniformCollectorBase
		, public TDecorators...
	{
		static_assert((std::is_base_of<IMeshUniformDataBase, TDecorators>::value && ...), 
			"All TDecorators must inherit from IMeshUniformDataBase struct.");

		virtual void SetAllUniforms(GLuint programId) override
		{
			(TDecorators::SetInternalUniforms(programId), ...);
		}
	};










	template<typename TVertex>
	struct MeshFeature_HasVerts
	{
		std::vector<TVertex> m_verts;
		GLuint m_vertexbuffer; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasIndicies
	{
		std::vector<int> m_indices;
		GLuint m_elementbuffer; //#todo: Giving each its own buffer might fragment memory
	};

	struct MeshFeature_HasNormals {}; //TODO
	struct MeshFeature_HasUVs {}; //TODO












	struct StaticMeshNoIndiciesSolidColor3
		: public IMesh
		, public MeshFeature_HasVerts<Vertex>
		, public IMeshUniformCollector<MeshUniform_SolidColor<fcolor3>>
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};
	struct StaticMeshNoIndiciesSolidColor3_2DTransform
		: public IMesh
		, public MeshFeature_HasVerts<Vertex>
		, public IMeshUniformCollector<MeshUniform_SolidColor<fcolor3>, MeshUniformCollection_2DTransform>
	{
		//TODO
	};



	struct StaticMeshSolidColor3
		: public IMesh
		, public MeshFeature_HasVerts<Vertex>
		, public MeshFeature_HasIndicies
		, public IMeshUniformCollector< MeshUniform_SolidColor<fcolor3> >
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};



	struct StaticMeshVertexColor3
		: public IMesh
		, public MeshFeature_HasVerts<VertexWithColor3>
		, public MeshFeature_HasIndicies
	{
		virtual void Init() override;
		virtual void Render(GLuint programId) override;
	};
}


